# ATRCConfig.cmake

# Summary of Exported Variables
# -----------------------------
# ATRC_FOUND              : Boolean indicating if the ATRC package is found.
# ATRC_VERSION            : The version of the ATRC package (e.g., 2.2.0).
# ATRC_INCLUDE_DIR        : Path to the ATRC include directory.
# ATRC_LIB_DEBUG          : Full path to the debug version of the ATRC library.
# ATRC_LIB_RELEASE        : Full path to the release version of the ATRC library.
# ATRC_DLL_DEBUG          : Full path to the debug version of the ATRC DLL (Windows only).
# ATRC_DLL_RELEASE        : Full path to the release version of the ATRC DLL (Windows only).
# ATRC_ARCH               : Architecture of the ATRC library (x64 or x86).


# Define the ATRC library root directory
set(ATRC_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

# Include directory for ATRC
set(ATRC_INCLUDE_DIR "${ATRC_ROOT}/include")

# Determine architecture (x64 or x86)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ATRC_ARCH "x64")
else()
    set(ATRC_ARCH "x86")
endif()

# Platform-specific library directories and files
if (WIN32)
    # Windows-specific configuration
    set(ATRC_LIB_DIR "${ATRC_ROOT}/Windows/${ATRC_ARCH}")
    set(ATRC_DEBUG_LIB_DIR "${ATRC_LIB_DIR}/Debug")
    set(ATRC_RELEASE_LIB_DIR "${ATRC_LIB_DIR}/Release")
    
    set(ATRC_LIB_DEBUG "ATRC.lib")
    set(ATRC_LIB_RELEASE "ATRC.lib")

    set(ATRC_DLL_DEBUG "ATRC.dll")
    set(ATRC_DLL_RELEASE "ATRC.dll")
    
    set(ATRC_PDB_DEBUG "ATRC.pdb")
    
    set(ATRC_EXP_DEBUG "ATRC.exp")
    set(ATRC_EXP_RELEASE "ATRC.exp")
elseif (UNIX)
    # Linux-specific configuration
    set(ATRC_LIB_DIR "${ATRC_ROOT}/Linux/${ATRC_ARCH}")
    set(ATRC_DEBUG_LIB_DIR "${ATRC_LIB_DIR}/Debug")
    set(ATRC_RELEASE_LIB_DIR "${ATRC_LIB_DIR}/Release")
    
    set(ATRC_LIB_DEBUG "libATRC.so")
    set(ATRC_LIB_RELEASE "libATRC.so")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Set complete paths for libraries and debug symbols
set(ATRC_LIB_DEBUG_PATH "${ATRC_DEBUG_LIB_DIR}/${ATRC_LIB_DEBUG}")
set(ATRC_LIB_RELEASE_PATH "${ATRC_RELEASE_LIB_DIR}/${ATRC_LIB_RELEASE}")
set(ATRC_DLL_DEBUG_PATH "${ATRC_DEBUG_LIB_DIR}/${ATRC_DLL_DEBUG}")
set(ATRC_DLL_RELEASE_PATH "${ATRC_RELEASE_LIB_DIR}/${ATRC_DLL_RELEASE}")

# Documentation files (optional use in consuming projects)
set(ATRC_DOCS_DIR "${ATRC_ROOT}/docs")
set(ATRC_DOCS_FILES
    "${ATRC_DOCS_DIR}/api_reference.htm"
    "${ATRC_DOCS_DIR}/ATRC_C-CPP.chm"
    "${ATRC_DOCS_DIR}/ATRC_C-CPP.pdf"
    "${ATRC_DOCS_DIR}/index.htm"
    "${ATRC_DOCS_DIR}/installation_guide.htm"
    "${ATRC_DOCS_DIR}/styles.css"
)

# Export variables for consuming projects
set(ATRC_FOUND          TRUE)
set(ATRC_VERSION        2.2.0)
set(ATRC_INCLUDE_DIR    ${ATRC_INCLUDE_DIR})
set(ATRC_LIB_DEBUG      ${ATRC_LIB_DEBUG_PATH})
set(ATRC_LIB_RELEASE    ${ATRC_LIB_RELEASE_PATH})
set(ATRC_DLL_DEBUG      ${ATRC_DLL_DEBUG_PATH})
set(ATRC_DLL_RELEASE    ${ATRC_DLL_RELEASE_PATH})
set(ATRC_ARCH           ${ATRC_ARCH})


# Include the CMake package configuration helpers
include(CMakePackageConfigHelpers)

# Generate a version file for compatibility
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ATRCConfigVersion.cmake"
    VERSION ${ATRC_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Export the ATRC package
export(PACKAGE ATRC)
