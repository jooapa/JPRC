#pragma once

#ifdef ATRC_EXPORTS
    #define ATRC_EXPORT __declspec(dllexport)
#else
    #define ATRC_EXPORT __declspec(dllimport)
#endif

// Declare the exported function
extern "C" ATRC_EXPORT int ATRCFunction();

extern "C" ATRC_EXPORT void Filehandler();