# Add executable for testing
add_executable(ATRC.Test 
    "ATRC.Test.cpp"
    "ATRC.Test.h"
)

# Link the test executable with the ATRC library
target_link_libraries(ATRC.Test PRIVATE ATRC)

# Set include directories (if needed)
include_directories("${PROJECT_SOURCE_DIR}/ATRC/include")

# Set C++ standard for tests (if CMake version is greater than 3.12)
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ATRC.Test PROPERTY CXX_STANDARD 20)
endif()

# Optional RPATH settings for UNIX-based systems
if (UNIX)
    set(CMAKE_BUILD_RPATH "${PROJECT_SOURCE_DIR}/ATRC/libs")
    set(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/ATRC/libs")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# Enable testing
enable_testing()

# Add custom test targets if needed
# add_custom_target(run_tests COMMAND ${CMAKE_BINARY_DIR}/ATRC.Test)
