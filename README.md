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

## Building using ready made scripts

If needed, you can set Visual studio environmental values by running ```batch\msvs-dev-env.bat```

Check that ```batch\globals.bat``` has the right paths
 - TOOLSPATH = Path to your tools directory containing Visual Studio dev-env scripts
 - CMAKEPATH = Path to your CMAKE
 - NINJAPATH = Path to your Ninja
 - COMPILERPATH = Path to your compiler

To start build process, run ```msvs-dev-cmd.bat```, this script builds ATRC and ATRC.Test and runs the latter one

Run ```batch\del.bat``` to reset CMake cache and return source folders to starting point
