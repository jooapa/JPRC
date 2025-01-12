# Determine the platform (Windows or Linux)
if (WIN32)
    set(CompilePlatform "Windows")
elseif (UNIX)
    set(CompilePlatform "Linux")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

# Detect architecture (x86 or x64)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(Arch "x64")
else()
    set(Arch "x86")
endif()

# Detect build configuration (Debug or Release)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(BuildConfig "Debug")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(BuildConfig "Release")
else()
    message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif()

# Construct ATRC directory path
set(ATRC_DIR "${CMAKE_SOURCE_DIR}/ATRC_2.0.0/${CompilePlatform}/${Arch}/${BuildConfig}")

# Find the ATRC library
find_library(ATRC NAMES ATRC REQUIRED PATHS ${ATRC_DIR})

# Include ATRC headers
include_directories("${CMAKE_SOURCE_DIR}/ATRC_2.0.0/include")

# Link the ATRC library
if (NOT TARGET ATRC_FOUND)
    message(FATAL_ERROR "Could not find ATRC library in ${ATRC_DIR}")
else()
    message(STATUS "Found ATRC library: ${ATRC}")
endif()
