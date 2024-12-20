#!/bin/bash

# Default options
CMAKE_OPTIONS="-DBUILD_PHYSICAL=ON"

# Override with user-provided arguments
if [ "$#" -gt 0 ]; then
  CMAKE_OPTIONS="$@"
fi

rm -r build
mkdir build
cd build
echo "CMake options: $CMAKE_OPTIONS"
cmake $CMAKE_OPTIONS ..
# cmake ..
make
