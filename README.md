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
    std::string filename = "test.atrc";
    std::shared_ptr<ATRC_FD> fd = Read(filename, "utf-8");
    fd->AutoSave = true;
    if (fd) {
        AddBlock(fd, "block_name");
        AddKey(fd, "block_name", "key_name", "value");
        AddKey(fd, "block_name", "key_name2", "value");
        RemoveKey(fd, "block_name", "key_name2");
        ModifyKey(fd, "block_name", "key_name2", "new_value");
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }

    return 0;
}
```

## Example resource file

```ini
%var_name%=value_1
<%priv_var>=value_2
[block_name]
key=value

! reference variables
reference=variable values are: %var_name%, %priv_var%

! insert variables
insert=insert a variable here: %*%

! insert whitespaces
whitespace=&whitespaces&

! insert reserved char
reserved=reserved characters: \%, \&, \!

! insert variables in a sequence. These start counting from 0
insert2=insert second variable here: %*1% and first one here: %*0*
```

## Building

```sh
    # Create build files:
    cmake --preset {preset_type} -B./out/{out_dir}/build -DCMAKE_BUILD_TYPE={Release|Debug} -DCMAKE_TOOLCHAIN_FILE=./cmake/{toolchain_file}.cmake

    # Example
    cmake --preset linux-x64-release -B./out/linux-x64-release/build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-linux-x64.cmake

    # Create release package
    sudo apt update
    sudo apt install cmake clang ninja-build lld mingw-w64 7zip
    chmod +x ./loop.sh
    ./loop.sh
```
