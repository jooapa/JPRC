#include <ATRC.h>
#include <filer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool Read(C_PATRC_FD self) {
    return _ATRC_WRAP_READ(self);
}
const char* ReadVariable(C_PATRC_FD self, const char* varname) {
    const char* res = "";
    return res;
}
const char* ReadKey(C_PATRC_FD self, const char* block, const char* key) {
    const char* res = "";

    return res;
}
bool DoesExistBlock(C_PATRC_FD self, const char* block) {
    return false;
}
bool DoesExistVariable(C_PATRC_FD self, const char* varname) {
    return false;
}
bool DoesExistKey(C_PATRC_FD self, const char* block, const char* key) {
    return false;
}
bool IsPublic(C_PATRC_FD self, const char* varname) {
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

C_PATRC_FD Create_ATRC_FD(const char *filename){
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
    self->AutoSave = false;
    //TODO
    free(self);

}
