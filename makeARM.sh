#!/bin/bash

while getopts ":d" option; do
    case $option in
    d)
        echo "Compiling in debug."
        mkdir -p ./build/Debug
        cd build/Debug
        cmake ../.. -D CMAKE_BUILD_TYPE=Debug -D CMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -D CMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ -DRELTYP="ARM" -DSQLITECPP_INTERNAL_SQLITE=ON
        make
        exit
        ;;
    *)
        ;;
    esac
done

echo "Compiling in release."
mkdir -p ./build/Release
cd build/Release
cmake ../.. -D CMAKE_BUILD_TYPE=Release -D CMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -D CMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ -DRELTYP="ARM" -DSQLITECPP_INTERNAL_SQLITE=ON
make