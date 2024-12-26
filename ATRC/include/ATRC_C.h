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

    #ifdef __cplusplus
    _ATRCFiledata(){Constructor_Empty(*this);}
    _ATRCFiledata(const char *path){Constructor(this, path);}
    ~_ATRCFiledata(){Destructor(this);}
    #endif // __cplusplus
} ATRC_FD, *PATRC_FD;

void Constructor_Empty(PATRC_FD *self);
bool Constructor(PATRC_FD *self, const char *path);
const char* ReadVariable(PATRC_FD self, const char* varname);
const char* ReadKey(PATRC_FD self, const char* block, const char* key);
bool DoesExistBlock(PATRC_FD self, const char* block);
bool DoesExistVariable(PATRC_FD self, const char* varname);
bool DoesExistKey(PATRC_FD self, const char* block, const char* key);
bool IsPublic(PATRC_FD self, const char* varname);
void InsertVar(PATRC_FD self, const char* line, const char** args);
const char* InsertVar_S(PATRC_FD self, const char* line, const char** args);
void AddBlock(PATRC_FD self, const char* blockname);
void RemoveBlock(PATRC_FD self, const char* blockname);
void AddVariable(PATRC_FD self, const char* varname, const char* value);
void RemoveVariable(PATRC_FD self, const char* varname);
void ModifyVariable(PATRC_FD self, const char* varname, const char* value);
void AddKey(PATRC_FD self, const char* block, const char* key, const char* value);
void RemoveKey(PATRC_FD self, const char* block, const char* key);
void ModifyKey(PATRC_FD self, const char* block, const char* key, const char* value);
void Destructor(PATRC_FD self);






_WRAPPER_EXIM_ bool _ATRC_WRAP_READ(PATRC_FD self);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ATRC_C_H