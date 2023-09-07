#!/bin/bash

mkdir -p /build/Release
mkdir -p /build/Debug

cd build/Release
cmake ../..
make