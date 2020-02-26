
cmake_minimum_required(VERSION 3.1.0)

project(libusbpp11 VERSION 0.0.1)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(examples)
add_subdirectory(doc)
add_subdirectory(src)

install(EXPORT libusbpp11 DESTINATION cmake)

if (UNIX)
	set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Pavol Kurina")
set(CPACK_PACKAGE_CONTACT "Pavol Kurina <pavol.kurina@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The ${PROJECT_NAME} package.")

if (UNIX)
	execute_process(COMMAND dpkg --print-architecture OUTPUT_VARIABLE CPACK_PACKAGE_ARCHITECTURE)
	string(STRIP "${CPACK_PACKAGE_ARCHITECTURE}" CPACK_PACKAGE_ARCHITECTURE)
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CPACK_PACKAGE_ARCHITECTURE}")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_SOURCE_GENERATOR "DEB")
else()
	set(CPACK_GENERATOR "ZIP") 
endif()

include(CPack)