#ifndef ATRC_H
#define ATRC_H

#ifdef ATRC_EXPORTS
#  define ATRC_API __declspec(dllexport)
#else
#  define ATRC_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

ATRC_API void ATRCFunction();

#ifdef __cplusplus
}
#endif

#endif // ATRC_H