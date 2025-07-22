# ATRC resource/configuration file library for C/C++

Configuration file library made with C++17, with a wrapper for C

## Documentation

See [.\docs\index.html](https://antonako1.github.io/ATRC/docs/index.html)

## Use with CMake

```cmake
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MyProject "main.cpp")

set(LIBS_DIR "${CMAKE_SOURCE_DIR}/libs")

# Add ATRC
set(ATRC_DIR "path/to/ATRC-2.3.0/Include")
find_package(ATRC REQUIRED)
target_link_libraries(MyProject PRIVATE ${ATRC_LIB_RELEASE})
target_include_directories(MyProject PRIVATE ${ATRC_INCLUDE_DIR})
```

## Example program in C++

```cpp
#include <ATRC.h>

int main() {
    std::cout << "ATRC Test Application" << std::endl;
    ATRC_FD fd = ATRC_FD("./test.atrc", ReadMode::ATRC_READ_ONLY);
    if (!fd.CheckStatus()) {
        std::cerr << "Failed to read ATRC file. Please check the file path and format." << std::endl;
        return 1;
    }
    fd.SetAutoSave(true);
    fd.SetWriteCheck(true);

    std::cout << "Running C++ tests" << std::endl;
    
    fd.AddBlock("TestBlock");
    fd.AddKey("TestBlock", "TestKey", "TestValue");

    fd.AddBlock("AnotherBlock");
    fd.AddKey("AnotherBlock", "AnotherKey", "AnotherValue");
    fd.AddKey("AnotherBlock", "AnotherKey2", "AnotherValue");

    fd.RemoveBlock("TestBlock");
    fd.RemoveBlock("AnotherBlock");

    fd.AddVariable("TestVariable", "TestValue");
    std::cout << fd["TestVariable"]["TestValue"] << std::endl;
    return 0;
}
```

## Example resource file

```ini
#!ATRC
# Example resource file
%username%=admin                # Define username
%domain%=atrc.com               # Define domain
%email%=%username%@%domain%     # Define email. Outputs: "admin@atrc.com"
[SystemData]                    # Define block
#.IF WINDOWS
CurrentOperatingSystem=Windows  # User machine: Windows
#.ELIF LINUX OR UNIX
CurrentOperatingSystem=Linux    # User machine: Linux or Unix
#.ELSE
#.ERROR Undefined platform      # Logs error message: "Undefined platform"
#.ENDIF
WelcomeMessage=Hi %username%, loading at: %*%\%&    # Once injected, outputs: "Hi admin, loading at: 50% " 
```

## Building & Running

Build and run with CMake. See cmake\README.md or run ready-made scripts

```bat
    .\vs_run.bat :: See usage
    .\scripts\run_tests.bat

    # Create release package
    # Requires: WSL2 debian, 7z, Windows build tools and Strawberry Perl
    .\scripts\build_and_package.bat
```
