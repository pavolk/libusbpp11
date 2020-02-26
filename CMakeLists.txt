
add_library(usbpp11 usb/usb.cpp)

target_include_directories(usbpp11
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/usb
)

target_link_libraries(usbpp11 PRIVATE CONAN_PKG::libusb)

install(TARGETS usbpp11
		EXPORT usbpp11
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
		ARCHIVE DESTINATION lib
)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include)
