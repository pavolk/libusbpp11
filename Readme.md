# Introduction

This is a thin C++ wrapper for [libusb](https://github.com/libusb/libusb.git). Unlike other such wrappers it tries to preserve the original libusb API, but taking advantage of C++ idioms like automatic ressource management with smart-pointers and exceptions.

There are no dependencies, besides the libusb and a compiler supporting C++11.

Knowing the libusb API, using libusbpp11 should feel familiar, but save a lot of the boilerplate code as shown in following example code:

## Libusb example:

```
#include <libusb.h>

int main(void)
{
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		libusb_exit(NULL);
		return (int) cnt;
	}

	// ...
	
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	return 0;
}
```

## Libusbpp11 example:

```
#include <usb.h>

int main(void)
{
	try {
		auto ctx = usb::init();
		auto devices = usb::get_devices(ctx);

		// ...
		
	} catch(const usb::system_error& ex) {
		std::cerr << ex.what() << std::endl;
		return ex.code().value();
	}
	
	return 0;
}
```
# Building from source

## Install the tools

* [CMake](https://cmake.org/download/) 
* [conan](https://conan.io/downloads.html)

## Get the sources

```
$ git clone https://github.com/pavolk/libusbpp11.git
$ cd libusbpp11
$ mkdir build && cd build
```

## Install dependencies with conan

```
$ conan install --build=missing ..
```

## Build and install

```
$ cmake --build . --target install
```

# Status 

This is by no means complete, but usable for simple libusb applications using the synchronous libusb API.

Pull requests are welcome! 

Enjoy,

Pavol.
