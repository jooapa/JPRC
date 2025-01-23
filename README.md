# ATRC resource/configuration file library for C/C++

Configuration file library made with C++17, with a wrapper for C

## Documentation

See [.\docs\index.htm](https://antonako1.github.io/ATRC/docs/index.htm)

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

```conf
#!ATRC
# define variable
%var_name%=value
# define private variable
<%priv_var>=value

# define block
[block_name]
# define key
key=value

# reference variables
reference=variable values are: %var_name%, %priv_var%
# output: "variable values are: value, value" 

# insert variables
insert=insert a variable here: %*%

# insert whitespaces
whitespace=&whitespaces&
# output: " whitespaces "

# insert reserved char
reserved=reserved characters: \%, \&, \#
# output: "reserved characters: %, &, #"

# insert variables in a sequence. These start counting from 0
insert2=insert second variable here: %*1% and first one here: %*0*

# Preprocessor tags:
# Ignore following amount of rows
#.IGNORE=<Amount_of_rows_to_skip>
#.IGNORE=1
THIS LINE IS IGNORED

# Comparisons. See docs for all values to use here
#.IF LINUX AND WINDOWS OR MACOS
user_info=Linux and Windows or MacOS in use!
#.ELIF LINUX AND X86
user_info=32 bit Linux!
#.ELSE
user_info=None
#.ENDIF

```

## Building & Running

```sh
    # Linux
    cmake --preset linux-x64-debug -B./out/linux-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-linux-x64.cmake -DATRC_BUILD_TESTS=ON && cp ./ATRC.Test/test.atrc ./out/linux-x64-debug/build/ATRC.Test/test.atrc && cmake --build ./out/linux-x64-debug/build --config Debug --target run_tests 

    # VS
    # 32-bit
    cmake -S . -B out\win_32 -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -A Win32 & cmake --build out\win_32
    # 64-bit
    cmake -S . -B out\win_64 -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -A x64 & cmake --build out\win_64

    # Create release package
    # Requires: debian, 7z, Windows build tools, Strawberry Perl, HTML Help Workshop
    .\scripts\build_and_package.bat
```
