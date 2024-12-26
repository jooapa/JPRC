# Your existing code remains mostly unchanged
add_library(ATRC SHARED 
    "filehandler.cpp"
    "usage.cpp"
    "ATRCFiledata.cpp"
    "atrc_stdlib.cpp"
    "include/ATRC.hpp"
    "include/filer.h"
    "snippets/InsertVar.cpp"
    "snippets/ReadKey.cpp"
    "snippets/ReadVariable.cpp"
    "snippets/DoesExist_var_key_block.cpp"
    "snippets/IsPublic.cpp"
    "snippets/to_save/blocks.cpp"
    "snippets/to_save/variables.cpp"
    "snippets/to_save/keys.cpp"
)

include_directories("./include")
target_include_directories(ATRC PUBLIC ${CMAKE_BINARY_DIR}/exports)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ATRC PROPERTY CXX_STANDARD 20)
endif()


