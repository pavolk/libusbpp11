# Introduction

This is a thin C++ wrapper for [libusb](https://github.com/libusb/libusb.git). Unlike some other C++ wrappers (e.g. [Libusbpp](https://github.com/zarthcode/Libusbpp)), which are at this moment more complete, this wrapper tries to preserve the original libusb API as much as possible, but taking advantage of C++ idioms like automatic ressource management with smart-pointers and exceptions.

Knowing the libusb API, using this library should feel familiar, but save most of the boilerplate code as shown in following example code:

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
#include <iostream>
#include <usb.h>

int main(void)
{
	try {
		auto ctx = usb::init();
		auto devices = usb::get_device_list(ctx);

		// ...
		
	} catch(const usb::system_error& ex) {
		std::cerr << ex.what() << std::endl;
		return ex.code().value();
	}
	
	return 0;
}
```

# Building from source

There are no dependencies, besides the libusb and a compiler supporting C++11. 

The dependencies are managed by the C++ package manager [conan](https://conan.io/downloads.html). The easiest way at the moment is to build the library from source by [CMake](https://cmake.org/download/) .

## Get the sources

```
$ git clone https://github.com/pavolk/libusbpp11.git
$ cd libusbpp11
$ mkdir build && cd build
```

## Install dependencies with conan

```
build$ conan install --build=missing ..
```

## Build and install

```
build$ cmake --build . --target install
```

# Status 

This is by no means complete, but usable for simple libusb applications using the synchronous libusb API.

# Documentation

There is not much of additional documentation at this moment, but most of the functions are just thin wrappers around the function with the same name of the libusb, which is extensively documented [here](http://libusb.sourceforge.net/api-1.0/). A brief look at the [code](src/usb.cpp) will probably answer most of the remaining questions.

More documentation will follow.


Pull requests are welcome! 

Enjoy,

Pavol.

