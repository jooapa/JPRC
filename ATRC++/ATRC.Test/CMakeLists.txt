cmake_minimum_required(VERSION 3.8)

project("ATRC.Test")

# Set ATRC import library directory
set(ATRC_DIR "${PROJECT_SOURCE_DIR}/libs")

# Find ATRC library
find_library(ATRC NAMES ATRC REQUIRED PATHS ${ATRC_DIR})
message(STATUS "ATRC library found at: ${ATRC}")

# Include header directory
include_directories("${PROJECT_SOURCE_DIR}/libs/include")

# Add executable and link with ATRC library
add_executable(ATRC.Test ATRC.Test.cpp ATRC.Test.h)
target_link_libraries(ATRC.Test PRIVATE ${ATRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ATRC.Test PROPERTY CXX_STANDARD 20)
endif()

if (UNIX)
  # Specify build and install RPATH
  set(CMAKE_BUILD_RPATH "${ATRC_DIR}")
  set(CMAKE_INSTALL_RPATH "${ATRC_DIR}")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# Optional: Specify additional build settings or targets here

