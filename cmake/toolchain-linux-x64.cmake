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
set(CMAKE_C_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "-fPIC")

# Any other platform-specific configurations can go here
