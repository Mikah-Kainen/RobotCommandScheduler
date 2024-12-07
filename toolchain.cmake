# Toolchain file: toolchain.cmake

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabi-g++")

# Set sysroot containing GLIBC 2.24-11+deb9u4
set(CMAKE_FIND_ROOT_PATH /glibc-2.24)

# Include path adjustments
set(CMAKE_INCLUDE_PATH "/glibc-2.24/include" ${CMAKE_INCLUDE_PATH})
set(CMAKE_LIBRARY_PATH "/glibc-2.24/lib" ${CMAKE_LIBRARY_PATH})