
/// @author: Pavol Kurina, pavol.kurina@gmail.com

#include <iostream>
#include <sstream>
#include "usb.h"

#define __func__ __FUNCTION__

namespace usb
{
	std::string error_message(const char* func, ssize_t error_code)
	{
		std::stringstream ss;
		ss << func << ": failed with " << std::dec << error_code
			<< " (" << libusb_error_name(static_cast<int>(error_code)) << ")";
		return ss.str();
	}

	static void exit(libusb_context* ctx)
	{
		libusb_exit(ctx);
	}

	Context init(int debug_level)
	{
		libusb_context* ctx = 0;
		int err = libusb_init(&ctx);
		if (err < 0) {
			throw system_error(error_message("libusb_init", err), static_cast<libusb_error>(err));
		}
		libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, debug_level);
		return Context(ctx, exit);
	}

	static void unref_device(libusb_device* dev)
	{
		libusb_unref_device(dev);
	}

	std::list<Device> get_device_list(Context ctx)
	{
		libusb_device** list;
		ssize_t size = libusb_get_device_list(ctx.get(), &list);
		if (size < 0) {
			throw system_error(error_message("libusb_get_device_list", size), static_cast<libusb_error>(size));
		}
		std::list<Device> retv;
		for (unsigned i = 0; i < static_cast<unsigned>(size); ++i) {
			retv.push_back(Device(list[i], unref_device));
		}
		libusb_free_device_list(list, 0/* don't unref devices */);
		return retv;
	}

	libusb_device_descriptor get_device_descriptor(Device device)
	{
		libusb_device_descriptor desc;
		int err = libusb_get_device_descriptor(device.get(), &desc);
		if (err < 0) {
			throw system_error(error_message("libusb_get_device_descriptor", err), static_cast<libusb_error>(err));
		}
		return desc;
	}

	uint16_t vendor_id(Device device)
	{
		return get_device_descriptor(device).idVendor;
	}

	uint16_t product_id(Device device)
	{
		return get_device_descriptor(device).idProduct;
	}

	unsigned get_device_address(Device device)
	{
		return libusb_get_device_address(device.get());
	}

	class HandleDeleter {
	public:
		HandleDeleter() : _ifno(-1) {}

		// Called from claim_interface(), which "upgraded" the handle to an interface-handle,
		// thus the deleter needs to call release_interface() before close().
		void release_interface_before_close(int ifno)
		{
			_ifno = ifno;
		}

		void operator()(libusb_device_handle* handle)
		{
			try_release_interface(handle);
			close(handle);
		}

	private:
		void try_release_interface(libusb_device_handle* handle)
		{
			if (-1 == _ifno) {
				// not a handle to an interface
				return;
			}
			int err = libusb_release_interface(handle, _ifno);
			if (err < 0) {
				std::cerr << "libusb_release_interface(" << handle << ", "
					<< std::dec << _ifno << ") failed." << std::endl;
			}
		}

		void close(libusb_device_handle* handle)
		{
			libusb_close(handle);
		}

		int _ifno;
	};

	Handle open(Device dev)
	{
		if (!dev) {
			throw std::invalid_argument("invalid reference to a device");
		}
		libusb_device_handle* handle = 0;
		int err = libusb_open(dev.get(), &handle);
		if (err < 0) {
			throw system_error(error_message("libusb_open", err), static_cast<libusb_error>(err));
		}
		return Handle(handle, HandleDeleter());
	}

	void claim_interface(Handle handle, unsigned interface_number)
	{
		if (!handle) {
			throw std::invalid_argument("Invalid handle.");
		}

		int err = libusb_claim_interface(handle.get(), interface_number);
		if (err < 0) {
			throw system_error(error_message("libusb_claim_interface", err), static_cast<libusb_error>(err));
		}
		std::get_deleter<HandleDeleter>(handle)->release_interface_before_close(interface_number);
	}

	unsigned get_max_packet_size(Handle handle, unsigned char endpoint_address)
	{
		auto device = libusb_get_device(handle.get());
		if (!device) {
			throw std::invalid_argument("Invalid handle.");
		}

		auto mps_or_err = libusb_get_max_packet_size(device, endpoint_address);
		if (mps_or_err < 0) {
			throw system_error(error_message("libusb_get_max_packet_size", mps_or_err), static_cast<libusb_error>(mps_or_err));
		}

		return mps_or_err;
	}

	static void free_config_descriptor(libusb_config_descriptor* desc)
	{
		libusb_free_config_descriptor(desc);
	}

	ConfigurationDescriptor get_configuration_descriptor(Device dev, unsigned value)
	{
		libusb_config_descriptor* desc = 0;

		if (!dev) {
			throw std::invalid_argument("Invalid device.");
		}

		int err = libusb_get_config_descriptor_by_value(dev.get(), static_cast<uint8_t>(value), &desc);
		if (err) {
			throw system_error(error_message("libusb_get_config_descriptor_by_value", err), static_cast<libusb_error>(err));
		}
		return ConfigurationDescriptor(desc, free_config_descriptor);
	}

	unsigned get_max_packet_size(Device dev, unsigned char endpoint)
	{
		return libusb_get_max_packet_size(dev.get(), endpoint);
	}

	size_t control_transfer(Handle handle, const ControlRequest& req, unsigned timeoutMillis)
	{
		if (!handle) {
			throw std::invalid_argument("Invalid handle.");
		}

		int err = libusb_control_transfer(handle.get(), req.bRequestType, req.bRequest, req.wValue, req.wIndex,
			static_cast<unsigned char*>(req.data), req.wLength, timeoutMillis);
		if (err < 0) {
			throw system_error(error_message("libusb_control_transfer", err), static_cast<libusb_error>(err));
		}
		return static_cast<size_t>(err);
	}

	size_t bulk_transfer(Handle interface_handle, unsigned char endpoint, void* data, size_t length, unsigned timeoutMillis)
	{
		if (!interface_handle) {
			throw std::invalid_argument("Invalid handle.");
		}

		int actual_length = 0;
		int err = libusb_bulk_transfer(interface_handle.get(), endpoint, static_cast<unsigned char*>(data), static_cast<int>(length),
			&actual_length, timeoutMillis);
		if (err < 0) {
			throw system_error(error_message("libusb_bulk_transfer", err), static_cast<libusb_error>(err));
		}

		return static_cast<size_t>(actual_length);
	}
}

std::ostream& operator<<(std::ostream& os, const libusb_device_descriptor& desc)
{
	os << "dev: cls=0x" << std::hex << static_cast<int>(desc.bDeviceClass)
		<< ", subcls=0x" << std::hex << static_cast<int>(desc.bDeviceSubClass)
		<< ", proto=0x" << std::hex << static_cast<int>(desc.bDeviceProtocol)
		<< ", vid=0x" << std::hex << desc.idVendor
		<< ", vid=0x" << std::hex << desc.idProduct
		;
	return os;
}

std::ostream& operator<<(std::ostream& os, const libusb_endpoint_descriptor& desc)
{
	os << "ep: address=0x" << std::hex << static_cast<int>(desc.bEndpointAddress)
		<< ", attrib=0x" << std::hex << static_cast<int>(desc.bmAttributes)
		<< ", mps=" << std::dec << desc.wMaxPacketSize
		;
	return os;
}

std::ostream& operator<<(std::ostream& os, const libusb_interface_descriptor& desc)
{
	os << "if: num=" << std::dec << static_cast<unsigned>(desc.bInterfaceNumber)
		<< ", altset=" << std::dec << static_cast<unsigned>(desc.bAlternateSetting)
		<< ", cls=0x" << std::hex << static_cast<unsigned>(desc.bInterfaceClass)
		<< ", subcls=0x" << std::hex << static_cast<unsigned>(desc.bInterfaceSubClass)
		<< ", proto=0x" << std::hex << static_cast<unsigned>(desc.bInterfaceProtocol)
		<< std::endl;
	for (int i = 0; i < desc.bNumEndpoints; ++i) {
		os << desc.endpoint[i] << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const libusb_interface& intf)
{
	for (int i = 0; i < intf.num_altsetting; ++i) {
		os << intf.altsetting[i];
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, usb::ConfigurationDescriptor config)
{
	os << "config: val=" << std::dec << static_cast<int>(config->bConfigurationValue)
		<< ", len=" << std::dec << static_cast<int>(config->wTotalLength)
		<< std::endl;
	for (int i = 0; i < config->bNumInterfaces; ++i) {
		os << config->interface[i];
	}
	return os;
}
