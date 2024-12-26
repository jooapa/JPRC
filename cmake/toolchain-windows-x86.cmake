set(CMAKE_SYSTEM_NAME Windows)

message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}, System: ${CMAKE_SYSTEM_NAME}, CMake: ${CMAKE_VERSION}")
message(STATUS "Windows 32-bit toolchain file")
# Specify MinGW compilers
# 32-bit settings
set(CMAKE_C_COMPILER "i686-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "i686-w64-mingw32-g++")

set(CMAKE_LINKER "i686-w64-mingw32-ld")

# Set MinGW paths
set(CMAKE_LIBRARY_PATH "/usr/i686-w64-mingw32/lib")
set(CMAKE_INCLUDE_PATH "/usr/i686-w64-mingw32/include")

# Target platform
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")

set(CMAKE_RC_COMPILER "i686-w64-mingw32-windres")

set(WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
if (WINDOWS_EXPORT_ALL_SYMBOLS)
    add_compile_definitions(WINDOWS_EXPORT_ALL_SYMBOLS)
endif()
# Optional: Windows SDK version
set(CMAKE_SYSTEM_VERSION 10.0)
