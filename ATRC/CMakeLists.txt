add_library(ATRC SHARED 
    "filehandler.cpp"
    "core_dev.cpp"
    "c.c"
    "c_cpp.cpp"
    "ATRCFiledata.cpp"
    "atrc_stdlib.cpp"
)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include_directories("./include")
target_include_directories(ATRC PUBLIC ${CMAKE_BINARY_DIR}/exports)
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ATRC PROPERTY CXX_STANDARD 17)
endif()
target_include_directories(ATRC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)