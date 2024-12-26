#!/bin/bash

# Define the root of your project
PROJECT_ROOT=$(pwd)

VERSION="2.0.0"
OUT_DIR="${PROJECT_ROOT}/out"

# Now loop through each preset in CMakePresets.json and build accordingly
declare -a presets=(
    # "linux-x64-debug" "linux-x64-release"
    # "linux-x86-debug" "linux-x86-release"
    "windows-x64-debug" "windows-x64-release"
    "windows-x86-debug" "windows-x86-release"
    # "macos-x64-debug" "macos-x64-release"
    # "macos-x86-debug" "macos-x86-release"
)

# Loop through each preset and run the corresponding CMake build command
for preset in "${presets[@]}"; do
    echo "Building preset: $preset"
    
    # Extract the build type from the preset name (e.g., debug or release)
    if [[ "$preset" == *"debug"* ]]; then
        build_type="Debug"
    else
        build_type="Release"
    fi
    
    # Define toolchain file based on the platform
    if [[ "$preset" == *"linux"* ]]; then
        if [[ "$preset" == *"x86"* ]]; then
            toolchain_file="${PROJECT_ROOT}/cmake/toolchain-linux-x86.cmake"
        else
            toolchain_file="${PROJECT_ROOT}/cmake/toolchain-linux-x64.cmake"
        fi
    elif [[ "$preset" == *"windows"* ]]; then
        if [[ "$preset" == *"x86"* ]]; then
            toolchain_file="${PROJECT_ROOT}/cmake/toolchain-windows-x86.cmake"
        else
            toolchain_file="${PROJECT_ROOT}/cmake/toolchain-windows-x64.cmake"
        fi
    elif [[ "$preset" == *"macos"* ]]; then
        if [[ "$preset" == *"x86"* ]]; then
            toolchain_file="${PROJECT_ROOT}/cmake/toolchain-macos-x86.cmake"
        else
            toolchain_file="${PROJECT_ROOT}/cmake/toolchain-macos-x64.cmake"
        fi
    fi
    
    # Run cmake to generate the build files with Ninja as the generator, using the toolchain file
    command="cmake --preset $preset -B${OUT_DIR}/${preset}/build -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_TOOLCHAIN_FILE=$toolchain_file"
    echo "Running command: $command"
    eval $command
    
    # Build all targets after generating the CMake files
    command="cmake --build ${OUT_DIR}/${preset}/build --config $build_type --parallel"
    echo "Running command: $command"
    eval $command
        
    echo "Completed building for preset: $preset"
done

echo "All builds completed!"


# Define the build directories
OUTPUT_DIR="${PROJECT_ROOT}/ATRC_${VERSION}"

# Define an array of the platforms and architectures you want to build for
declare -a platforms=(
    # "Linux" 
    "Windows"
    # "macOS"
 )
declare -a architectures=("x86" "x64")
declare -a build_types=("Debug" "Release")

# Create the directories for each platform, architecture, and build type
for platform in "${platforms[@]}"; do
    for arch in "${architectures[@]}"; do
        for build_type in "${build_types[@]}"; do
            # Define the target directory path
            OUT_DIR="${OUTPUT_DIR}/${platform}/${arch}/${build_type}"
            
            # Create the necessary directories
            mkdir -p "$OUT_DIR"
            echo "Created directory: $OUT_DIR"

            # Copy the built libraries to the target directory
            IN_DIR="${PROJECT_ROOT}/out/${platform,}-${arch,,}-${build_type,,}/build/ATRC"
            FILENAME_IN="libATRC"
            if [[ "$build_type" == "Debug" ]]; then
                prefix="d_"
            else
                prefix=""
            fi
            FILENAME_OUT="${prefix}ATRC_${platform}_${arch,,}"
            
            echo \n $FILENAME_IN, $FILENAME_OUT, $IN_DIR

            # Copy the library files
            if [[ "$platform" == "Windows" ]]; then
                # a -> def
                nm "${IN_DIR}/${FILENAME_IN}.dll.a" > "${IN_DIR}/${FILENAME_IN}.def"
                if [[ "$arch" == "x86" ]]; then
                    x86_64-w64-mingw32-dlltool -d "${IN_DIR}/${FILENAME_IN}.def" -l "${IN_DIR}/${FILENAME_IN}.dll.a" -D "${IN_DIR}/${FILENAME_IN}.dll"
                    echo "x86_64-w64-mingw32-dlltool -d ${IN_DIR}/${FILENAME_IN}.def -l ${IN_DIR}/${FILENAME_IN}.dll.a -D ${IN_DIR}/${FILENAME_IN}.dll"
                else
                    i686-w64-mingw32-dlltool -d "${IN_DIR}/${FILENAME_IN}.def" -l "${IN_DIR}/${FILENAME_IN}.dll.a" -D "${IN_DIR}/${FILENAME_IN}.dll"
                fi
                # cp -r "${IN_DIR}/${FILENAME_IN}.dll" "${OUT_DIR}/${FILENAME_OUT}.dll"
                # cp -r "${IN_DIR}/${FILENAME_IN}.lib" "${OUT_DIR}/${FILENAME_OUT}.lib"
            fi
        done
    done
done
mkdir -p "$OUTPUT_DIR/docs"
mkdir -p "$OUTPUT_DIR/include"


# Copy stuff
cp -r "${PROJECT_ROOT}/ATRC/include/ATRC.hpp" "${OUTPUT_DIR}/include/ATRC.hpp"
cp -r "${PROJECT_ROOT}/docs/docs.md" "${OUTPUT_DIR}/docs/docs.md"
cp -r "${PROJECT_ROOT}/LICENSE.txt" "${OUTPUT_DIR}/LICENSE.txt"