#pragma once

#ifndef ATRC_C_H
#define ATRC_C_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "ATRC_COMMON.hpp"


typedef struct C_Variable {
    const char *Name;
    const char *Value;
    bool IsPublic;
} C_Variable, *C_PVariable;

typedef struct C_Key {
    const char *Name;
    const char *Value;
} C_Key, *C_PKey;


typedef struct C_Block {
    const char *Name;
    C_Key *Keys;
    uint64_t KeyCount;
} C_Block, *C_PBlock;

typedef struct _ATRCFiledata{
    C_Variable *Variables;
    C_Block *Blocks;
    const char *Filename;
    bool AutoSave;

    bool (*ReadFiledata)(struct _ATRCFiledata *self);
    const char* (*ReadVariable)(struct _ATRCFiledata *self, const char* varname);
    const char* (*ReadKey)(struct _ATRCFiledata *self, const char* block, const char* key);
    bool (*DoesExistBlock)(struct _ATRCFiledata *self, const char* block);
    bool (*DoesExistVariable)(struct _ATRCFiledata *self, const char* varname);
    bool (*DoesExistKey)(struct _ATRCFiledata *self, const char* block, const char* key);
    bool (*IsPublic)(struct _ATRCFiledata *self, const char* varname);
    void (*InsertVar)(struct _ATRCFiledata *self, const char* line, const char** args);
    const char* (*InsertVar_S)(struct _ATRCFiledata *self, const char* line, const char** args);
    void (*AddBlock)(struct _ATRCFiledata *self, const char* blockname);
    void (*RemoveBlock)(struct _ATRCFiledata *self, const char* blockname);
    void (*AddVariable)(struct _ATRCFiledata *self, const char* varname, const char* value);
    void (*RemoveVariable)(struct _ATRCFiledata *self, const char* varname);
    void (*ModifyVariable)(struct _ATRCFiledata *self, const char* varname, const char* value);
    void (*AddKey)(struct _ATRCFiledata *self, const char* block, const char* key, const char* value);
    void (*RemoveKey)(struct _ATRCFiledata *self, const char* block, const char* key);
    void (*ModifyKey)(struct _ATRCFiledata *self, const char* block, const char* key, const char* value);
    void (*Destructor)(struct _ATRCFiledata *self);
} ATRC_FD, *PATRC_FD;

ATRC_API PATRC_FD CreateEmptyFiledata();
ATRC_API PATRC_FD CreateFiledata(const char *path);
ATRC_API bool ReadFiledata(PATRC_FD self, const char *path);
ATRC_API void DestroyFiledata(PATRC_FD *self);

ATRC_API int test(void);

_WRAPPER_EXIM_ bool _ATRC_WRAP_READ(PATRC_FD self);
_WRAPPER_EXIM_ void _ATRC_WRAP_ERRORMSG(int err_num, int line_number, const char *var_name, const char *filename);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ATRC_C_H