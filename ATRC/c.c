#include <ATRC.h>
#include <filer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool Read(C_PATRC_FD self) {
    return _ATRC_WRAP_FUNC_1(self);
}

const char* ReadVariable(C_PATRC_FD self, const char* varname) {
    const char* res = "";
    for(uint64_t i = 0; i < self->Variables->VariableCount; i++){
        if(strcmp(varname, self->Variables->Variables[i].Name) == 0){
            if(self->Variables->Variables[i].IsPublic) {
                res = self->Variables->Variables[i].Value;
            } else {
                _ATRC_WRAP_FUNC_2(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, self->Filename);
                res = "";
            }
        }
    }
    return res;
}
const char* ReadKey(C_PATRC_FD self, const char* block, const char* key) {
    const char* res = "";
    for(uint64_t i = 0; i < self->Blocks->BlockCount; i++){
        if(strcmp(block, self->Blocks->Blocks[i].Name) == 0){
            for(uint64_t j = 0; j < self->Blocks->Blocks[i].KeyCount; j++){
                if(strcmp(key, self->Blocks->Blocks[i].Keys[j].Name) == 0){
                    res = self->Blocks->Blocks[i].Keys[j].Value;
                    return res;
                }
            }
        }
    }
    return res;
}
bool DoesExistBlock(C_PATRC_FD self, const char* block) {
    for(uint64_t i = 0; i < self->Blocks->BlockCount; i++){
        if(strcmp(block, self->Blocks->Blocks[i].Name) == 0) return true;
    }
    return false;
}
bool DoesExistVariable(C_PATRC_FD self, const char* varname) {
    for(uint64_t i = 0; i < self->Variables->VariableCount; i++){
        if(strcmp(varname, self->Variables->Variables[i].Name) == 0){
            if(self->Variables->Variables[i].IsPublic) return true;
            else{
                _ATRC_WRAP_FUNC_2(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, self->Filename);
                return false;            
            }
        }
    }
    return false;
}
bool DoesExistKey(C_PATRC_FD self, const char* block, const char* key) {
    for(uint64_t i = 0; i < self->Blocks->BlockCount; i++){
        if(strcmp(block, self->Blocks->Blocks[i].Name) == 0){
            for(uint64_t j = 0; j < self->Blocks->Blocks[i].KeyCount; j++){
                if(strcmp(key, self->Blocks->Blocks[i].Keys[j].Name) == 0) return true;
            }
        }
    }
    return false;
}
bool IsPublic(C_PATRC_FD self, const char* varname) {
    for(uint64_t i = 0; i < self->Variables->VariableCount; i++){
        if(strcmp(varname, self->Variables->Variables[i].Name) == 0) return self->Variables->Variables[i].IsPublic;
    }
    return false;
}
void InsertVar(C_PATRC_FD self, const char* line, const char** args) {

}
const char* InsertVar_S(C_PATRC_FD self, const char* line, const char** args) {
    return "";
}
void AddBlock(C_PATRC_FD self, const char* blockname) {}
void RemoveBlock(C_PATRC_FD self, const char* blockname) {}
void AddVariable(C_PATRC_FD self, const char* varname, const char* value) {}
void RemoveVariable(C_PATRC_FD self, const char* varname) {}
void ModifyVariable(C_PATRC_FD self, const char* varname, const char* value) {}
void AddKey(C_PATRC_FD self, const char* block, const char* key, const char* value) {}
void RemoveKey(C_PATRC_FD self, const char* block, const char* key) {}
void ModifyKey(C_PATRC_FD self, const char* block, const char* key, const char* value) {}

C_PATRC_FD Create_ATRC_FD(char *filename){
    C_PATRC_FD res = Create_Empty_ATRC_FD();
    if(res == NULL) return NULL;
    res->Filename = filename;
    if(!Read(res)) return NULL;
    return res;
}
C_PATRC_FD Create_Empty_ATRC_FD(void){
    C_PATRC_FD res = (C_PATRC_FD)malloc(sizeof(C_ATRC_FD));
    if(res == NULL) return NULL;
    res->AutoSave = false;
    res->Variables = NULL;
    res->Blocks = NULL;
    return res;
}


void Destroy_ATRC_FD(C_PATRC_FD self) {
    //TODO: Destroy blocks, variables and filename


    free(self->Filename);
    self->Filename = NULL;
    free(self);
    self = NULL;
}
