# ATRC resource file library made

## Use with CMake

```
cmake_minimum_required(VERSION 3.15)
project(MyProject)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ATRC_2.0.0/cmake")

# Include the FindATRC script
include(FindATRC)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Link the ATRC library
target_link_libraries(${PROJECT_NAME} PRIVATE ${ATRC})

# Include ATRC headers
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/ATRC_2.0.0/include")
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
		return;
	}
    std::cout << fd["var_name"] << std::endl;

    C_PATRC_FD c_fd = fd.ToCStruct();
    const char* line = fd["block_name"]["key"];
    const char* args[] = {"Hello everyone", nullptr};
    std::cout << "Before: " << line << std::endl;
    char* res = InsertVar_S(line.c_str(), args);
    std::cout << "After: " << res << std::endl;
    delete[] res;
    return 1;
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
    cmake --preset linux-x64-debug -B./out/linux-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-linux-x64.cmake -DATRC_BUILD_TESTS=ON && cmake --build ./out/linux-x64-debug/build --config Debug --target run_tests

    cmake --preset windows-x64-debug -B./out/windows-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-windows-x64.cmake -DATRC_BUILD_TESTS=ON & cmake --build ./out/windows-x64-debug/build --config Debug --target run_tests


    # VS
    cmake --preset VS2022-x64-debug -B./out/VS2022-x64-debug/build -DCMAKE_BUILD_TYPE=Debug -DATRC_BUILD_TESTS=ON & .\out\VS2022-x64-debug\build\ATRC.sln

    # Create release package
    # Requires: debian wsl,7z,strawberry perl,wkhtmltopdf
    .\scripts\fullbuild.bat
```
