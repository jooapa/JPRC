add_library(ATRC SHARED 
    "filehandler.cpp"
    "c.c"
    "cpp.cpp"
    "atrc_stdlib.cpp"
    "ATRCFiledata.cpp"
)

include_directories("./include")
target_include_directories(ATRC PUBLIC ${CMAKE_BINARY_DIR}/exports)
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ATRC PROPERTY CXX_STANDARD 20)
endif()