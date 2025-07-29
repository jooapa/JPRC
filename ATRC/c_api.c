#define _CRT_SECURE_NO_DEPRECATE
#include "./include/ATRC.h"
#include "./include/filer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



uint64_t GetEnumValue(PATRC_FD fd, const char* block, const char* key) {

}

const char* ReadVariable(PATRC_FD fd, const char* varname) {

}
const char* ReadKey(PATRC_FD fd, const char* block, const char* key) {

}
bool DoesExistBlock(PATRC_FD fd, const char* block) {

}
bool DoesExistVariable(PATRC_FD fd, const char* varname) {

}
bool DoesExistKey(PATRC_FD fd, const char* block, const char* key) {

}
bool IsPublic(PATRC_FD fd, const char* varname) {

}
/*
NOTE: If added someday, check _ATRC_WRAP_INSERTVAR imementation for correct usage.
void InsertVar(char* line, const char** args) {
    _ATRC_WRAP_INSERTVAR(line, args);
}
*/
const char* InsertVar_S(const char* line, const char** args) {
    return _ATRC_WRAP_INSERTVAR_S(line, args);
}
bool AddBlock(PATRC_FD fd, const char* blockname) {

}

bool RemoveBlock(PATRC_FD fd, const char* blockname) {

}

bool AddVariable(PATRC_FD fd, const char* varname, const char* value) {

}

bool RemoveVariable(PATRC_FD fd, const char* varname) {

}

bool ModifyVariable(PATRC_FD fd, const char* varname, const char* value) {

}

bool AddKey(PATRC_FD fd, const char* block, const char* key, const char* value) {

}

bool RemoveKey(PATRC_FD fd, const char* block, const char* key) {

}

bool ModifyKey(PATRC_FD fd, const char* block, const char* key, const char* value) {

}

bool WriteCommentToBottom(PATRC_FD fd, const char* comment) {

}

bool WriteCommentToTop(PATRC_FD fd, const char* comment) {

}

bool Read(PATRC_FD fd, const char* path, ReadMode mode) {
    if(fd->Filename != NULL) {
        __ATRC_FREE_MEMORY(fd->Filename);
        fd->Filename = NULL;
    }
    fd->Filename = __STRDUP(path);
    return _ATRC_WRAP_READ(fd, path, mode);
}

PATRC_FD Create_ATRC_FD(const char *filename, ReadMode mode){
    PATRC_FD fd = Create_Empty_ATRC_FD();
    if(fd == NULL) {
        return NULL;
	}
    fd->Filename = __STRDUP(filename);
    if(!Read(fd, filename, mode)) {
        Destroy_ATRC_FD(fd);
        return NULL;
    }
    return fd;
}
PATRC_FD Create_Empty_ATRC_FD(void){
    PATRC_FD fd = (PATRC_FD)malloc(sizeof(ATRC_FD));
    if(fd == NULL) {
        errormsg(ERR_MEMORY_ALLOCATION_FAILED, __LINE__, "ATRC_FD", "Create_Empty_ATRC_FD");
        return NULL;
	}
    fd->AutoSave = false;
    fd->Writecheck = false;
    fd->Filename = NULL;

    fd->BlockArray.BlockCount = 0;
    fd->BlockArray.Blocks = NULL;

    fd->VariableArray.VariableCount = 0;
    fd->VariableArray.Variables = NULL;
    return fd;
}

void Destroy_ATRC_FD_Variables(PATRC_FD fd) {
    for (uint64_t i = 0; i < fd->VariableArray.VariableCount; i++) {
        PVariable var = &fd->VariableArray.Variables[i];
        __ATRC_FREE_MEMORY(var->Name);
        __ATRC_FREE_MEMORY(var->Value);
    }
    __ATRC_FREE_MEMORY(fd->VariableArray.Variables);
}

void Destroy_ATRC_FD_Blocks_And_Keys(PATRC_FD fd) {
    for (uint64_t i = 0; i < fd->BlockArray.BlockCount; i++) {
		PBlock block = &fd->BlockArray.Blocks[i];
		__ATRC_FREE_MEMORY(block->Name);
		block->Name = NULL;
        for (uint64_t j = 0; j < block->KeyArray.KeyCount; j++) {
            PKey key = &block->KeyArray.Keys[j];
			__ATRC_FREE_MEMORY(key->Name);
			__ATRC_FREE_MEMORY(key->Value);
        }
		__ATRC_FREE_MEMORY(block->KeyArray.Keys);
    }
	__ATRC_FREE_MEMORY(fd->BlockArray.Blocks);
}

void Destroy_ATRC_FD(PATRC_FD fd) {
    Destroy_ATRC_FD_Blocks_And_Keys(fd);
	Destroy_ATRC_FD_Variables(fd);
    __ATRC_FREE_MEMORY(fd->Filename);
    __ATRC_FREE_MEMORY(fd);
}

void ATRC_FREE_MEMORY(void **ptr) {
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;  // This updates the caller's pointer
    }
}
