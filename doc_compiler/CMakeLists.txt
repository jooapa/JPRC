# Add executable for testing
add_executable(DOC_COMPILER
    "Compiler.cpp"
    "Compiler.h"
)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Link the test executable with the ATRC library
target_link_libraries(DOC_COMPILER PRIVATE ATRC)

# Set include directories (if needed)
include_directories("${PROJECT_SOURCE_DIR}/ATRC/include")

# Set C++ standard for tests (if CMake version is greater than 3.12)
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


# Enable testing
enable_testing()

# For MSVC Debugging
if (MSVC)
    message(STATUS "Copying ATRC DLLs to binary directory")
    add_custom_command(
        TARGET DOC_COMPILER POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_BINARY_DIR}/ATRC/Debug/ATRC.dll"
        "${CMAKE_BINARY_DIR}/DOC_COMPILER/Debug/ATRC.dll"
        COMMENT "Copying ATRC DLLs to binary directory"
    )
endif()
# Copy the test.atrc file to the binary directory
add_custom_command(
    TARGET DOC_COMPILER POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/test.atrc"
    "${CMAKE_BINARY_DIR}/DOC_COMPILER/test.atrc"
    COMMENT "Copying test.atrc to binary directory"
)



# Add the custom test targets
add_custom_target(compile_docs
    COMMAND "${CMAKE_BINARY_DIR}/DOC_COMPILER/DOC_COMPILER"
    DEPENDS "DOC_COMPILER" "ATRC"
    COMMENT "Running tests for ATRC"
)

# If you want the test target to be built by default when invoking `make`, set it as a default target
add_dependencies(compile_docs DOC_COMPILER ATRC)
