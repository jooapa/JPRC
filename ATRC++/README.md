# ATRC resource file library made with C/C++

## Example program in C++


```cpp
#include "ATRC.h"

int main(){
    
    return 0;
}
```

## Example resource file

```
[block_name]
    key1 = 1 
    key2 = lol 
    key3 = true 
```

## Building using ready made scripts

If needed, you can set Visual studio environmental values by running ```msvs-dev-env.bat```

Check that ```globals.bat``` has the right paths
 - TOOLSPATH = Path to your tools directory containing Visual Studio dev-env scripts
 - CMAKEPATH = Path to your CMAKE
 - NINJAPATH = Path to your Ninja
 - COMPILERPATH = Path to your compiler

To start build process, run ```msvs-dev-cmd.bat```, this script builds ATRC and ATRC.Test and runs the latter one

Run ```del.bat``` to reset CMake cache and return source folders to starting point