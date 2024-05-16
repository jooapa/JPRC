cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ATRC.Test")

# Find ATRC import library
set(ATRC_DIR "${PROJECT_SOURCE_DIR}/libs")  # Assuming libs folder is in project source
find_library(ATRC NAMES ATRC REQUIRED PATHS ${ATRC_DIR})

# Include header directory
include_directories("${PROJECT_SOURCE_DIR}/libs/include")

# Add executable and link with ATRC library
add_executable (ATRC.Test "ATRC.Test.cpp" "ATRC.Test.h")
target_link_libraries(ATRC.Test PRIVATE ATRC)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ATRC.Test PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
