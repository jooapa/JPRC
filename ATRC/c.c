#include <DEV_INC.hpp>
bool Read(PATRC_FD self){
    return _ATRC_WRAP_READ(self);
}
void Constructor_Empty(PATRC_FD *self){
    *self = (PATRC_FD)malloc(sizeof(ATRC_FD));
    (*self)->Variables = NULL;
    (*self)->Blocks = NULL;
    (*self)->Filename = NULL;
    (*self)->AutoSave = false;
}

bool Constructor(PATRC_FD *self, const char *path){
    Destroy(self);
    Constructor_Empty(self);
    (*self)->Filename = path;
    return Read(*self);
}

const char* ReadVariable(PATRC_FD self, const char* varname){
    const char* res = "";
    for(int i = 0; i < sizeof(self->Variables); i++){
        if(strcmp(self->Variables[i].Name, varname) == 0){
            if(self->Variables[i].IsPublic) {
                res = self->Variables[i].Value;
            } else {
                errormsg(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, self->Filename);
                res = "";
            }
            return res;
        }
    }
    return res;
}
const char* ReadKey(PATRC_FD self, const char* block, const char* key){
    const char* res = "";
    for(int i = 0; i < sizeof(self->Blocks); i++){
        if(strcmp(self->Blocks[i].Name, block) == 0){
            for(int j = 0; j < sizeof(self->Blocks[i].Keys); j++){
                if(strcmp(self->Blocks[i].Keys[j].Name, key) == 0){
                    res = self->Blocks[i].Keys[j].Value;
                    return res;
                }
            }
        }
    }
    return res;
}
bool DoesExistBlock(PATRC_FD self, const char* block){
    for(int i = 0; i < sizeof(self->Blocks); i++){
        if(strcmp(self->Blocks[i].Name, block) == 0) return true;
    }
    return false;
}
bool DoesExistVariable(PATRC_FD self, const char* varname){
    for(int i = 0; i < sizeof(self->Variables); i++){
        if(strcmp(self->Variables[i].Name, varname) == 0) return true;
    }
    return false;
}
bool DoesExistKey(PATRC_FD self, const char* block, const char* key){
    for(int i = 0; i < sizeof(self->Blocks); i++){
        if(strcmp(self->Blocks[i].Name, block) == 0){
            for(int j = 0; j < sizeof(self->Blocks[i].Keys); j++){
                if(strcmp(self->Blocks[i].Keys[j].Name, key) == 0) return true;
            }
        }
    }
    return false;
}
bool IsPublic(PATRC_FD self, const char* varname){
    for(int i = 0; i < sizeof(self->Variables); i++){
        if(strcmp(self->Variables[i].Name, varname) == 0){
            if(self->Variables[i].IsPublic) return true;
            else{
                errormsg(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, self->Filename);
                return false;            
            }
        } 
    }
    return false;
}
void InsertVar(PATRC_FD self, const char* line, const char** args){

}
const char* InsertVar_S(PATRC_FD self, const char* line, const char** args){

}
void AddBlock(PATRC_FD self, const char* blockname){}
void RemoveBlock(PATRC_FD self, const char* blockname){}
void AddVariable(PATRC_FD self, const char* varname, const char* value){}
void RemoveVariable(PATRC_FD self, const char* varname){}
void ModifyVariable(PATRC_FD self, const char* varname, const char* value){}
void AddKey(PATRC_FD self, const char* block, const char* key, const char* value){}
void RemoveKey(PATRC_FD self, const char* block, const char* key){}
void ModifyKey(PATRC_FD self, const char* block, const char* key, const char* value){}

void Destructor(PATRC_FD self){
    if(self->Variables != NULL) free(self->Variables);
    if(self->Blocks != NULL) {
        for(int i = 0; i < sizeof(self->Blocks); i++){
            if(self->Blocks[i].Keys != NULL) free(self->Blocks[i].Keys);
        }
        free(self->Blocks);
    }
    if (self->Filename) {
        free((void*)self->Filename);
    }
    free(self);
}