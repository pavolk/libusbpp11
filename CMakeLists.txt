cmake_minimum_required(VERSION 3.1.0)

project(libusbpp11 VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set debug library postfix" FORCE)

# if included from a parent project, this will fail...
if (NOT CONAN_DEPENDENCIES)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup(TARGETS)
endif ()

set(CMAKE_INSTALL_BINDIR bin)
set(CMAKE_INSTALL_LIBDIR lib)
set(CMAKE_INSTALL_INCLUDEDIR include)
if (WIN32)
	set(CMAKE_INSTALL_DATADIR data)
	set(CMAKE_INSTALL_DOCDIR doc/${PROJECT_NAME}-${PROJECT_VERSION})
else ()
	set(CMAKE_INSTALL_DATADIR share/${PROJECT_NAME}-${PROJECT_VERSION})
	set(CMAKE_INSTALL_DOCDIR share/doc/${PROJECT_NAME}-${PROJECT_VERSION})
endif()

add_subdirectory(doc)
add_subdirectory(examples)
add_subdirectory(include)
add_subdirectory(src)

install(EXPORT libusbpp11 
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
		FILE libusbpp11.cmake)

install(FILES Readme.md ChangeLog.md LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})
