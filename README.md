# ATRC resource file library made

## Use with CMake

```
if (WIN32)
    set(ATRC_DIR "${PROJECT_SOURCE_DIR}/ATRC/libs/win")
elseif (UNIX)
    set(ATRC_DIR "${PROJECT_SOURCE_DIR}/ATRC/libs/linux")
endif()

find_library(ATRC NAMES ATRC REQUIRED PATHS ${ATRC_DIR})
include_directories("${PROJECT_SOURCE_DIR}/atrc/include")
target_link_libraries(${project_name} PRIVATE ${ATRC})
```

## Example program in C++


```cpp
#include <ATRC.hpp>

int main()
{

}
```

## Example resource file

```ini
#__ATRC__
%var_name%=value_1
<%priv_var>=value_2
[block_name]
key=value

# reference variables
reference=variable values are: %var_name%, %priv_var%

# insert variables
insert=insert a variable here: %*%

# insert whitespaces
whitespace=&whitespaces&

# insert reserved char
reserved=reserved characters: \%, \&, \#

# insert variables in a sequence. These start counting from 0
insert2=insert second variable here: %*1% and first one here: %*0*
```

## Building & Running

```sh
    # Create build files:
    cmake --preset {preset_type} -B./out/{out_dir}/build -DCMAKE_BUILD_TYPE={Release|Debug} -DCMAKE_TOOLCHAIN_FILE=./cmake/{toolchain_file}.cmake

    # Build the files
    cmake --build ./out/{out_dir}/build --config {Release|Debug}

    # Run test program
    cmake --preset linux-x64-debug -B./out/linux-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-linux-x64.cmake -DATRC_BUILD_TESTS=ON && cmake --build ./out/linux-x64-debug/build --config Debug --target run_tests

    cmake --preset windows-x64-debug -B./out/windows-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-windows-x64.cmake -DATRC_BUILD_TESTS=ON & cmake --build ./out/windows-x64-debug/build --config Debug --target run_tests


    # VS
    cmake --preset VS2022-x64-debug -B./out/VS2022-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DATRC_BUILD_TESTS=ON & .\out\VS2022-x64-debug\build\ATRC.sln

    # Create release package
    # run in wsl
    sudo apt update
    sudo apt install cmake clang ninja-build lld mingw-w64 7zip gcc-multilib g++-multilib libc6-dev-i386
    # windows binaries: https://strawberryperl.com/
    chmod +x ./loop.sh
    ./loop.sh
```
