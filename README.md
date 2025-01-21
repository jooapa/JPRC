# ATRC resource/configuration file library for C/C++

## Documentation

See [.\docs\index.htm](https://antonako1.github.io/ATRC/docs/index.htm)

Uses C++17 standards

## Use with CMake

```bash

# Define the path to the ATRC configuration file
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ATRC_2.2.0/cmake")

# Find ATRC library using its config file
find_package(ATRC REQUIRED)

# Define project
project(MyProject)
add_executable(MyProject src/main.cpp)

# Include header files
target_include_directories(MyProject PRIVATE ${ATRC_INCLUDE_DIR})
```

## Example program

```cpp
#include <ATRC.h>

using namespace atrc;
int main()
{
    ATRC_FD fd = ATRC_FD("file.atrc");
    if (!fd.CheckStatus()) {
		std::cout << "File parsed unsuccesfully!" << std::endl;
		return 1;
	}
    std::cout << fd["var_name"] << std::endl;
    return 0;
}
```

## Example resource file

```ini
#ATRC
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
    cmake --preset linux-x64-debug -B./out/linux-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-linux-x64.cmake -DATRC_BUILD_TESTS=ON && cp ./ATRC.Test/test.atrc ./out/linux-x64-debug/build/ATRC.Test/test.atrc && cmake --build ./out/linux-x64-debug/build --config Debug --target run_tests 

    cmake --preset windows-x64-debug -B./out/windows-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-windows-x64.cmake -DATRC_BUILD_TESTS=ON & cmake --build ./out/windows-x64-debug/build --config Debug --target run_tests


    # VS
    cmake --preset VS2022-x64-debug -B./out/VS2022-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DATRC_BUILD_TESTS=ON & .\out\VS2022-x64-debug\build\ATRC.sln

    # Create release package
    # Requires: debian wsl,7z,strawberry perl,wkhtmltopdf,html tools
    .\scripts\fullbuild.bat
```
