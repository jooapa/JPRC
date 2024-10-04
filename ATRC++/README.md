# ATRC resource file library made with C/C++. Works with windows, propably with linux

Everything else work but you can't save changes into the files from c++

## Use with CMake

```
if (WIN32)
    set(ATRC_DIR "${PROJECT_SOURCE_DIR}/ATRC/libs/win")
elseif (UNIX)
    set(ATRC_DIR "${PROJECT_SOURCE_DIR}/ATRC/libs/linux")
endif()

find_library(ATRC NAMES ATRC REQUIRED PATHS ${ATRC_DIR})
include_directories("${PROJECT_SOURCE_DIR}/atrc/include")
target_link_libraries(${project_name} PRIVATE ${ATRC_DIR})
```

## Example program in C++


```cpp
#include <ATRC.h>

int main(){
    std::unique_ptr<ATRCFiledata> fd = Read("path_to_file", "utf-8", "");
    
    // Read key from a block
    std::string block_name = "settings";
    std::string key_name = "user_name";
    std::string key_contents = "";

    if(DoesExistBlock(block_name)){
        ReadKey(fd.get(), block_name, key_name, key_contents);
        std::cout << key_contents << std::endl;
    } else {
        std::cout << "Block [" << block_name << "] does not exist" << std::endl;
    }

    // Read variable
    std::string variable_name = "os";
    std::string variable_contents = "";
    if(
        DoesExistVariable(fd.get(), variable_name) 
        && IsPublic(fd.get(), variable_name))
    {
        ReadVariable(fd.get(), variable_name, variable_contents);
    } else {
        variable_contents = "windows_nt";
        AddVariable(fd.get(), variable_name, variable_contents);
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
