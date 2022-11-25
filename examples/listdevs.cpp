
/// @author Pavol Kurina, pavol.kurina@gmail.com

#include <iostream>
#include <libusbpp11/usb.h>

static usb::Handle try_open(usb::Device dev) 
{
	try {
		return usb::open(dev);
	} catch(...) {
		return usb::Handle();
	}
}

int main(void)
{
	try {
		auto ctx = usb::init();
		auto devices = usb::get_device_list(ctx);

		std::cout << "found " << devices.size() << " device(s):" << std::endl;
		
		for(auto& dev : devices) {
			auto dd = usb::get_device_descriptor(dev);
			std::cout << dd << std::endl;
			try {
				std::cout << usb::get_configuration_descriptor(dev) << std::endl;
				auto handle = try_open(dev);
				if (handle) {
					std::cout << "SerialNumber: \"" << usb::get_string(handle, dd.iSerialNumber) << "\"" << std::endl;
				}
			} catch(const usb::system_error& ex) {
				std::cerr << ex.what() << std::endl << std::endl;
			}
		}
		
	} catch(const usb::system_error& ex) {
		std::cerr << ex.what() << std::endl;
		return ex.code().value();
	}
	
	return 0;
}
