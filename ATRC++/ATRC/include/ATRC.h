#ifndef ATRC_H
#define ATRC_H

#ifdef ATRC_EXPORTS
#  define ATRC_API __declspec(dllexport)
#else
#  define ATRC_API __declspec(dllimport)
#endif

extern "C" ATRC_API void ATRCFunction();


#endif // ATRC_H
