
/// @author: Pavol Kurina, pavol.kurina@gmail.com
/// @brief Thin C++ wrapper for libusb.

#ifndef UTILS_USB_USB_H
#define UTILS_USB_USB_H

#include <memory>
#include <list>
#include <system_error>

#if defined(WIN32)
#pragma warning(disable: 4200)
#pragma warning(disable: 4510)
#pragma warning(disable: 4512)
#pragma warning(disable: 4610)
#endif
#include <libusb-1.0/libusb.h>

namespace usb
{

	typedef std::shared_ptr<libusb_context> Context;
	typedef std::shared_ptr<libusb_device> Device;
	typedef std::shared_ptr<libusb_device_handle> Handle;
	typedef std::shared_ptr<libusb_config_descriptor> ConfigurationDescriptor;

	class system_error : public std::system_error {
		typedef std::system_error Base;
	public:
		system_error(const std::string& msg, libusb_error error_code)
			: Base(error_code, std::system_category(), msg)
		{}
	};

	Context init(int debug_level = 0);

	std::list<Device> get_devices(Context ctx);

	libusb_device_descriptor get_device_descriptor(Device device);

	uint16_t vendor_id(Device device);
	uint16_t product_id(Device device);
	unsigned get_device_address(Device device);

	Handle open(Device dev);

	void claim_interface(Handle device_handle, unsigned interface_number);

	ConfigurationDescriptor get_configuration_descriptor(Device dev, unsigned value = 1);

	unsigned get_max_packet_size(Device dev, unsigned char endpoint_address);

	unsigned get_max_packet_size(Handle handle, unsigned char endpoint_address);

	struct ControlRequest {
		uint8_t bRequestType;
		uint8_t bRequest;
		uint16_t wValue;
		uint16_t wIndex;
		void* data;
		uint16_t wLength;
	};
	size_t control_transfer(Handle handle, const ControlRequest& req, unsigned timeoutMillis);

	size_t bulk_transfer(Handle interface_handle, unsigned char endpoint, void* data, size_t length, unsigned timeoutMillis);

}

std::ostream& operator<<(std::ostream& os, const libusb_device_descriptor& desc);
std::ostream& operator<<(std::ostream& os, const libusb_endpoint_descriptor& desc);
std::ostream& operator<<(std::ostream& os, const libusb_interface_descriptor& desc);
std::ostream& operator<<(std::ostream& os, const libusb_interface& intf);
std::ostream& operator<<(std::ostream& os, usb::ConfigurationDescriptor config);

#endif // UTILS_USB_USB_H
