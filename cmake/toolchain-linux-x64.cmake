# Toolchain file for Linux
message(STATUS "Linux 64-bit toolchain file")

# Specify the compilers
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_LINKER "ld.lld")
set(CMAKE_SYSTEM_NAME Linux)


# Define shared libraries build (ensure SO files are built)
set(BUILD_SHARED_LIBS ON)

# Set additional flags for Linux
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "-fPIC -O0 -Wall -Wextra -g")
    set(CMAKE_CXX_FLAGS "-fPIC -O0 -Wall -Wextra -g")
else()
    set(CMAKE_C_FLAGS "-fPIC -O3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "-fPIC -O3 -Wall -Wextra")
endif()

# Any other platform-specific configurations can go here
