cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ATRC")

# Include GenerateExportHeader module
include(GenerateExportHeader)

# Create the dynamic-link library (DLL)
add_library (ATRC SHARED 
    "main.cpp"
    "src/filehandler.cpp"
    "include/ATRC.h")


target_include_directories(ATRC PUBLIC ${CMAKE_BINARY_DIR}/exports)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Generate export header for the "ATRC" library
generate_export_header(ATRC EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/ATRC.h)


# Set C++ standard (if CMake version is greater than 3.12)
if (CMAKE_VERSION VERSION_GREATER 3.12)
set_property(TARGET ATRC PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
