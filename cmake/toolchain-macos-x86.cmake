set(CMAKE_SYSTEM_NAME Darwin)
message(STATUS "MacOS 32-bit toolchain file loaded")
# Specify compilers
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_LINKER "/usr/bin/ld")

# Set macOS-specific flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.15")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.15")

# Optional: Define macOS SDK
set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")

# Optional: Set macOS target version
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
