#!/bin/bash

mkdir -p /build/Release
mkdir -p /build/Debug

cd build/Release
cmake ../.. -D CMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -D CMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ -DRELTYP="ARM"
make