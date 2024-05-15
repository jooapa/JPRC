#pragma once

#ifdef ATRC_EXPORTS
    #define ATRC_API __declspec(dllexport)
#else
    #define ATRC_API __declspec(dllimport)
#endif

// Declare the exported function
extern "C" ATRC_API void ATRCFunction();

extern "C" ATRC_API void Filehandler();