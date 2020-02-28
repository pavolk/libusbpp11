
add_library(usbpp11 usb/usb.cpp)

target_include_directories(usbpp11
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
	PRIVATE
        	${CMAKE_CURRENT_SOURCE_DIR}/usb
)

target_link_libraries(usbpp11 PUBLIC CONAN_PKG::libusb)

install(TARGETS usbpp11
	EXPORT libusbpp11
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include)
