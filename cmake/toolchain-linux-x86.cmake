# Toolchain file for Linux
message(STATUS "Linux 32-bit toolchain file")
# Specify the compilers
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_LINKER "ld")
set(CMAKE_SYSTEM_NAME Linux)


# Define shared libraries build (ensure SO files are built)
set(BUILD_SHARED_LIBS ON)
# set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH} /usr/lib32 /lib32")
# Set additional flags for Linux
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden -O0 -g -m32 -Wall -Wextra -ggdb3")
    # set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden -O0 -g -m32 -Wall -Wextra -ggdb3")
    set(CMAKE_C_FLAGS "-fPIC -O0 -Wall -Wextra -g")
    set(CMAKE_CXX_FLAGS "-fPIC -O0 -Wall -Wextra -g")
else()
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden -O3 -m32 -Wall -Wextra -ggdb3")
    # set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden -O3 -m32 -Wall -Wextra -ggdb3")
    set(CMAKE_C_FLAGS "-fPIC -O3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "-fPIC -O3 -Wall -Wextra")
endif()

# Any other platform-specific configurations can go here
