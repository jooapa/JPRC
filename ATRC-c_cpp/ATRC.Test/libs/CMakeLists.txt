message("-- Linking ATRC_LIB")
add_library(ATRC_LIB SHARED ATRC.dll)
set_target_properties(ATRC_LIB PROPERTIES
    IMPORTED_LOCATION "libs/ATRC.dll"
    LINKER_LANGUAGE CPP
)