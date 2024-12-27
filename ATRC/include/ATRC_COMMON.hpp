#pragma once
#ifndef ATRC_COMON_HPP
#define ATRC_COMON_HPP


#ifdef ATRC_EXPORTS
#   if defined(_WIN32) || defined(_WIN64) || defined(WINDOWS_EXPORT_ALL_SYMBOLS)
#       define ATRC_API __declspec(dllexport)
#       define _WRAPPER_EXIM_ __declspec(dllexport)
#   elif __GNUC__ >= 4
#       define ATRC_API __attribute__((visibility("default")))
#       define _WRAPPER_EXIM_ __attribute__((visibility("default")))
#   endif
#endif

#ifndef ATRC_API
#   define ATRC_API 
#endif
#ifndef _WRAPPER_EXIM_
#   define _WRAPPER_EXIM_ 
#endif

#define FILEHEADER "#!__ATRC__\0"

#include <stdint.h>
#define __atrc__one__of__two__(cpp, c) \
    __atrc__one__of__two_helper__(cpp, c)
    
#ifdef __cplusplus
    #define __atrc__one__of__two_helper__(cpp, c) cpp
#else
    #define __atrc__one__of__two_helper__(cpp, c) c
#endif



#ifdef __cplusplus
#  include <vector>
#  include <string>
#  include <memory>
// Disable warning C4251 for std::vector and std::string.
#  if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER) 
#    pragma warning(push)
#    pragma warning(disable: 4251)
#  endif // _WIN32 || _WIN64

#  define __atrc__str__ std::string
#  define __atrc__bool__ bool
#else // __cplusplus
#  include <stdbool.h>
#  include <stdlib.h>
#  include <string.h>
#  define __atrc__str__ const char*
#  define __atrc__bool__ bool
#endif // __cplusplus

#ifdef INCLUDE_ATRC_STDLIB

#define     SUCCESSFULL_ACTION      1
#define     UNSUCCESFULL_ACTION     0

ATRC_API extern int atrc_stdlib_errval;

#ifdef __cplusplus
ATRC_API std::vector<std::string> atrc_to_vector(char separator, const char *value);
#else 
ATRC_API char** atrc_to_list(char separator, const char *value);
#endif // __cplusplus

ATRC_API bool atrc_to_bool(const char *value);

ATRC_API uint64_t atrc_to_uint64_t(const char *value);

ATRC_API int64_t atrc_to_int64_t(const char *value);

ATRC_API double atrc_to_double(const char *value);

#endif // INCLUDE_ATRC_STDLIB


#endif // ATRC_COMON_HPP