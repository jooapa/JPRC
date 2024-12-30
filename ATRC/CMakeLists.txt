add_library(ATRC SHARED 
    "filehandler.cpp"
    "core_dev.cpp"
    "c.c"
    "c_cpp.cpp"
    "ATRCFiledata.cpp"
    "atrc_stdlib.cpp"
)

include_directories("./include")
target_include_directories(ATRC PUBLIC ${CMAKE_BINARY_DIR}/exports)
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ATRC PROPERTY CXX_STANDARD 20)
endif()
target_include_directories(ATRC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)