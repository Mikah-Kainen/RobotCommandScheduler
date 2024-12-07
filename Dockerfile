# Use a base image with a compatible version of Debian or Ubuntu
#FROM debian:11-bullseye
FROM ubuntu:20.04
#FROM ev3cc

# Set non-interactive mode to avoid prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install essential tools
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-arm-linux-gnueabi \
    g++-arm-linux-gnueabi \
    wget \
    bison \
    gawk \
    texinfo \
    gcc \
    make \
    curl \
    git \
    cmake


# Set the working directory
WORKDIR /src

# Download, extract, build, and install GLIBC
RUN rm -rf glibc-2.24 && \
    wget http://ftp.gnu.org/gnu/libc/glibc-2.24.tar.gz && \
    tar -xzvf glibc-2.24.tar.gz && \
    cd glibc-2.24 && \
    mkdir build && cd build && \
    ../configure --prefix=/glibc-2.24 && \
    make -j$(nproc) && \
    make install

# Set up environment variables to use the new toolchain
ENV PATH="/glibc-2.24/bin:$PATH"
ENV SYSROOT="/glibc-2.24"
ENV LD_LIBRARY_PATH="/glibc-2.24/lib:$LD_LIBRARY_PATH"

# Add sysroot to CMake find paths
ENV CMAKE_INCLUDE_PATH="$SYSROOT/include"
ENV CMAKE_LIBRARY_PATH="$SYSROOT/lib"

# Copy your source code into the container
COPY . .

# Run CMake to configure and build your project
# You can run this step manually after launching the container


# build with: docker build -t my-glibc-2.24-builder .
# run with: docker run -it -v $(pwd):/src my-glibc-2.24-builder