
## Building using ready made scripts

If needed, you can set Visual studio environmental values by running ```msvs-dev-env.bat```

Check that ```globals.bat``` has the right paths
 - TOOLSPATH = Path to your tools directory containing Visual Studio dev-env scripts
 - CMAKEPATH = Path to your CMAKE
 - NINJAPATH = Path to your Ninja
 - COMPILERPATH = Path to your compiler

To start build process, run ```msvs-dev-cmd.bat```, this script builds ATRC and ATRC.Test and runs the latter one

Run ```del.bat``` to reset CMake cache and return source folders to starting point