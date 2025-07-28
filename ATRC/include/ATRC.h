/*+++
Header file for the ATRC (Advanced Tagged Resource Configuration Library) library.
  This file contains the declarations for the ATRC library, which provides functionality
for reading, writing, and manipulating configuration files in a structured format.
  This library is designed to be used in both C and C++ projects, with a focus on
ease of use and flexibility.

This file is part of the ATRC library project.
Author(s): Antonako1.

Licensed under the BSD 2-Clause License.
See the LICENSE file in the project root for license information.
---*/
#pragma once
#ifndef ATRC_H
#define ATRC_H
#if defined(_WIN32) || defined(_WIN64) || defined(WINDOWS_EXPORT_ALL_SYMBOLS)
#   ifdef ATRC_EXPORTS
#       define ATRC_API __declspec(dllexport)
#       define _WRAPPER_EXIM_ __declspec(dllexport)
#   else // ATRC_EXPORTS
#       define ATRC_API __declspec(dllimport)
#       define _WRAPPER_EXIM_ __declspec(dllimport)
#   endif // ATRC_EXPORTS
#endif // defined(_WIN32) || defined(_WIN64) || defined(WINDOWS_EXPORT_ALL_SYMBOLS)

#ifndef ATRC_API
#   define ATRC_API 
#endif // ATRC_API
#ifndef _WRAPPER_EXIM_
#   define _WRAPPER_EXIM_ 
#endif // _WRAPPER_EXIM_

#define FILEHEADER ""
#define __atrc__one__of__two__(cpp, c) \
    __atrc__one__of__two_helper__(cpp, c)
#define __nmsp__    namespace atrc {
#define __nmspe__   }
#ifdef __cplusplus
    #define __atrc__one__of__two_helper__(cpp, c) cpp
#else
    #define __atrc__one__of__two_helper__(cpp, c) c
#endif


#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum _ReadMode {
    ATRC_READ_ONLY,
    ATRC_CREATE_READ,
    ATRC_FORCE_READ,
} ReadMode;


typedef struct _Variable {
    char *Name;
    char *Value;
    bool IsPublic;
    uint64_t line_number;
} Variable, *PVariable;

typedef struct _Variable_Arr {
    Variable *Variables;
    uint64_t VariableCount;
} Variable_Arr, *PVariable_Arr;

typedef struct _Key {
    char *Name;
    char *Value;
    uint64_t line_number;
    uint64_t enum_value;
} Key, *PKey;

typedef struct _Block {
    char *Name;
    Key *Keys;
    uint64_t KeyCount;
    uint64_t line_number;
} Block, *PBlock;

typedef struct _Block_Arr {
    Block *Blocks;
    uint64_t BlockCount;
} Block_Arr, *PBlock_Arr;



typedef struct _ATRCFiledata{
    PVariable_Arr Variables;
    PBlock_Arr Blocks;
    char *Filename;
    bool AutoSave;
    bool Writecheck;
} ATRC_FD, *PATRC_FD;

ATRC_API bool Read(PATRC_FD self, const char* path, ReadMode readMode);
ATRC_API const char* ReadVariable(PATRC_FD self, const char* varname);
ATRC_API const char* ReadKey(PATRC_FD self, const char* block, const char* key);
ATRC_API bool DoesExistBlock(PATRC_FD self, const char* block);
ATRC_API bool DoesExistVariable(PATRC_FD self, const char* varname);
ATRC_API bool DoesExistKey(PATRC_FD self, const char* block, const char* key);
ATRC_API bool IsPublic(PATRC_FD self, const char* varname);
ATRC_API char* InsertVar_S(const char* line, const char** args);
ATRC_API bool AddBlock(PATRC_FD self, const char* blockname);
ATRC_API bool RemoveBlock(PATRC_FD self, const char* blockname);
ATRC_API bool AddVariable(PATRC_FD self, const char* varname, const char* value);
ATRC_API bool RemoveVariable(PATRC_FD self, const char* varname);
ATRC_API bool ModifyVariable(PATRC_FD self, const char* varname, const char* value);
ATRC_API bool AddKey(PATRC_FD self, const char* block, const char* key, const char* value);
ATRC_API bool RemoveKey(PATRC_FD self, const char* block, const char* key);
ATRC_API bool ModifyKey(PATRC_FD self, const char* block, const char* key, const char* value);
ATRC_API bool WriteCommentToBottom(PATRC_FD self, const char* comment);
ATRC_API bool WriteCommentToTop(PATRC_FD self, const char* comment);
ATRC_API uint64_t GetEnumValue(PATRC_FD self, const char* block, const char* key);

ATRC_API PATRC_FD Create_ATRC_FD(const char *filename, ReadMode readMode);
ATRC_API PATRC_FD Create_Empty_ATRC_FD();
ATRC_API void *Destroy_ATRC_FD_Blocks_And_Keys(PATRC_FD self);
ATRC_API void *Destroy_ATRC_FD_Variables(PATRC_FD self);
ATRC_API void *Destroy_ATRC_FD(PATRC_FD self);

ATRC_API void *ATRC_FREE_MEMORY(void *ptr);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus




/*+++
Start of ATRC standard library
---*/

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/*
Inject macro for C
USAGE:
{
    const char *res = NULL;
    ATRC_CINJECT("INPUT %*%, %*%\n", "arg1", "arg2", ...) // -> "INPUT arg1, arg2\n"
    // use res as needed
    free(res);
}
*/
#define ATRC_INJECT(line, ...) \
if(line != NULL) { \
    const char *message = line; \
    const char *args[] = {__VA_ARGS__, NULL}; \
    res = InsertVar_S(message, args); \
} \
else { \
    res = NULL; \
} \


/*+++
ATRC STDLIB:
    - atrc_to_list      (C & C++)
    - atrc_free_list    (C & C++)
    - atrc_to_bool      (C & C++)
    - atrc_to_uint64_t  (C & C++)
    - atrc_to_int64_t   (C & C++)
    - atrc_to_double    (C & C++)
---*/

enum ATRC_ERR {
	_ATRC_SUCCESSFULL_ACTION = 0,
	_ATRC_UNSUCCESSFULL_ACTION = 1
};

extern uint64_t atrc_stdlib_errval;
extern bool atrc_stdlib_writecheck;

typedef struct _String_Arr {
    char **list;
    uint64_t count;
} String_Arr, *PString_Arr;

ATRC_API PString_Arr atrc_to_list(const char separator, const char* value);

ATRC_API void *atrc_free_list(PString_Arr list);

ATRC_API bool atrc_to_bool(const char* value);

ATRC_API uint64_t atrc_to_uint64_t(const char* value);

ATRC_API int64_t atrc_to_int64_t(const char* value);

ATRC_API double atrc_to_double(const char* value);

ATRC_API double atrc_math_exp(const char* value);

#ifdef __cplusplus
} // Extern "C"
#endif // __cplusplus

#endif // ATRC_H