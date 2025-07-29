# ATRCTargets.cmake
# Usage:
#   set(ATRC_DIR "/path/to/ATRC/cmake")
#   
#   find_package(ATRC REQUIRED)
#   
#   add_executable(MyProject main.cpp)
#   
#   target_link_libraries(MyProject PRIVATE ATRC::ATRC)

# Create imported target
add_library(ATRC::ATRC SHARED IMPORTED)

# Set target properties
set_target_properties(ATRC::ATRC PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ATRC_INCLUDE_DIR}"
    IMPORTED_LOCATION_RELEASE "${ATRC_LIB_RELEASE}"
    IMPORTED_LOCATION_DEBUG "${ATRC_LIB_DEBUG}"
)

# Windows only: set IMPORTED_IMPLIB and DLLs (optional)
if(WIN32)
    set_target_properties(ATRC::ATRC PROPERTIES
        IMPORTED_IMPLIB_RELEASE "${ATRC_LIB_RELEASE}"
        IMPORTED_IMPLIB_DEBUG "${ATRC_LIB_DEBUG}"
        IMPORTED_LOCATION_RELEASE "${ATRC_DLL_RELEASE}"
        IMPORTED_LOCATION_DEBUG "${ATRC_DLL_DEBUG}"
    )
endif()
