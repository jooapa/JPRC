#!/bin/bash
cd "$(dirname "$0")"

source ./shell/globals.sh
./shell/build.sh


# build test
cd ATRC.Test
if [ ! -d "libs" ]; then
    mkdir libs
fi
cp ../ATRC/out/libATRC.so libs
LD_LIBRARY_PATH=libs:$LD_LIBRARY_PATH
if [ ! -d "libs/include" ]; then
    mkdir libs/include
fi
cp ../ATRC/include/ATRC.hpp ./libs/include

if [ ! -d "out" ]; then
    mkdir out
fi

cd out
cmake -G "Ninja" -DCMAKE_C_COMPILER="$COMPILER_C_PATH" -DCMAKE_CXX_COMPILER="$COMPIlER_CXX_PATH" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_INSTALL_PREFIX="./install/x64-debug" -DCMAKE_MAKE_PROGRAM="$NINJAPATH" ..
cmake --build . --config Debug

if [ $? -ne 0 ]; then
    echo "Build failed, exiting..."
    exit 1
fi
cd ..
./out/ATRC.Test
