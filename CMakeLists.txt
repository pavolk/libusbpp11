cmake_minimum_required(VERSION 3.1.0)

project(libusbpp11 VERSION 0.0.1)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(examples)
add_subdirectory(doc)
add_subdirectory(src)

install(EXPORT libusbpp11 DESTINATION cmake)
install(FILES Readme.md ChangeLog.md LICENSE DESTINATION .)
