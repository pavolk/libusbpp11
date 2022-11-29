
add_library(usbpp11 usb.cpp)

target_include_directories(usbpp11
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include> # REVISIT: ${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
       	${CMAKE_CURRENT_SOURCE_DIR}
)

if (WIN32)
	target_link_libraries(usbpp11 PUBLIC CONAN_PKG::libusb)
else ()
	target_include_directories(usbpp11 PRIVATE ${libusb_INCLUDE_DIRS})
	target_link_libraries(usbpp11 ${libusb_LIBRARIES})
endif ()



install(TARGETS usbpp11
		EXPORT libusbpp11
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
