#!/bin/sh
echo "Building project..."

cd "$(dirname "$0")"

. ./globals.sh

cd ../ATRC

mkdir -p out
cd out

# For release:
if [ "$1" = "Release" ]; then
    echo "Building release..."
    cmake -G "Ninja" -DCMAKE_C_COMPILER="$COMPILER_C_PATH" -DCMAKE_CXX_COMPILER="$COMPIlER_CXX_PATH" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX="./install/x64-release" -DCMAKE_MAKE_PROGRAM="$NINJAPATH" ..
    cmake --build . --config Release
    if [ $? -ne 0 ]; then
        echo "Build failed, exiting..."
        exit 1
    fi
    cd ..
    exit 0
fi

# For debug:
cmake -G "Ninja" -DCMAKE_C_COMPILER="$COMPILER_C_PATH" -DCMAKE_CXX_COMPILER="$COMPIlER_CXX_PATH" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_INSTALL_PREFIX="./install/x64-debug" -DCMAKE_MAKE_PROGRAM="$NINJAPATH" ..
cmake --build . --config Debug

if [ $? -ne 0 ]; then
    echo "Build failed, exiting..."
    exit 1
fi

cd ..