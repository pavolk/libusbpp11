
/// @author Pavol Kurina, pavol.kurina@gmail.com

#include <iostream>
#include <usb.h>

int main(void)
{
	try {
		auto ctx = usb::init();
		auto devices = usb::get_device_list(ctx);

		std::cout << "found " << devices.size() << " device(s):" << std::endl;
		
		for(auto& dev : devices) {
			std::cout << usb::get_device_descriptor(dev) << std::endl;
			try {
				std::cout << usb::get_configuration_descriptor(dev) << std::endl;
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
