mkdir -p ./build_normal/Release
mkdir -p ./build_normal/Debug

cd build_normal/Release
cmake ../.. -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++
make