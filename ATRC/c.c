#include <DEV_INC.hpp>
void Destructor(PATRC_FD* self) {
    if ((*self)->Variables != NULL) free((*self)->Variables);
    if ((*self)->Blocks != NULL) {
        for (int i = 0; i < sizeof(C_Block) / sizeof((*self)->Blocks[0]); i++) {
            if ((*self)->Blocks[i].Keys != NULL) {
                free((*self)->Blocks[i].Keys);
            }
        }
        free((*self)->Blocks);
    }
    if ((*self)->Filename) {
        free((void*)(*self)->Filename);
    }
    if (*self != NULL) free(self);
}
bool Read(PATRC_FD self) {
    return _ATRC_WRAP_READ(self);
}
bool Constructor_Empty(PATRC_FD* self) {
    Destructor(self);
    *self = (PATRC_FD)malloc(sizeof(ATRC_FD));
    if (*self == NULL) return false;
    (*self)->Variables = NULL;
    (*self)->Blocks = NULL;
    (*self)->Filename = NULL;
    (*self)->AutoSave = false;
    return true;
}

bool Constructor(PATRC_FD* self, const char* path) {
    Destructor(self);
    if (!Constructor_Empty(self)) return false;
    (*self)->Filename = path;
    return Read(*self);
}

const char* ReadVariable(PATRC_FD self, const char* varname) {
    const char* res = "";
    for (int i = 0; i < sizeof(C_Variable) / sizeof(self->Variables[0]); i++) {
        if (strcmp(self->Variables[i].Name, varname) == 0) {
            if (self->Variables[i].IsPublic) {
                res = self->Variables[i].Value;
            }
            else {
                _ATRC_WRAP_ERRORMSG(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, self->Filename);
                res = "";
            }
            return res;
        }
    }
    return res;
}
const char* ReadKey(PATRC_FD self, const char* block, const char* key) {
    const char* res = "";
    for (int i = 0; i < sizeof(C_Block) / sizeof(self->Blocks[0]); i++) {
        if (strcmp(self->Blocks[i].Name, block) == 0) {
            for (int j = 0; j < sizeof(C_Key) / sizeof(self->Blocks[i].Keys[0]); j++) {
                if (strcmp(self->Blocks[i].Keys[j].Name, key) == 0) {
                    res = self->Blocks[i].Keys[j].Value;
                    return res;
                }
            }
        }
    }
    return res;
}
bool DoesExistBlock(PATRC_FD self, const char* block) {
    for (int i = 0; i < sizeof(C_Block) / sizeof(self->Blocks[0]); i++) {
        if (strcmp(self->Blocks[i].Name, block) == 0) return true;
    }
    return false;
}
bool DoesExistVariable(PATRC_FD self, const char* varname) {
    for (int i = 0; i < sizeof(C_Variable) / sizeof(self->Variables[0]); i++) {
        if (strcmp(self->Variables[i].Name, varname) == 0) return true;
    }
    return false;
}
bool DoesExistKey(PATRC_FD self, const char* block, const char* key) {
    for (int i = 0; i < sizeof(C_Block) / sizeof(self->Blocks[0]); i++) {
        if (strcmp(self->Blocks[i].Name, block) == 0) {
            for (int j = 0; j < sizeof(C_Key) / sizeof(self->Blocks[i].Keys[0]); j++) {
                if (strcmp(self->Blocks[i].Keys[j].Name, key) == 0) return true;
            }
        }
    }
    return false;
}
bool IsPublic(PATRC_FD self, const char* varname) {
    for (int i = 0; i < sizeof(C_Variable) / sizeof(self->Variables[0]); i++) {
        if (strcmp(self->Variables[i].Name, varname) == 0) {
            if (self->Variables[i].IsPublic) return true;
            else {
                _ATRC_WRAP_ERRORMSG(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, self->Filename);
                return false;
            }
        }
    }
    return false;
}
void InsertVar(PATRC_FD self, const char* line, const char** args) {

}
const char* InsertVar_S(PATRC_FD self, const char* line, const char** args) {
    return "";
}
void AddBlock(PATRC_FD self, const char* blockname) {}
void RemoveBlock(PATRC_FD self, const char* blockname) {}
void AddVariable(PATRC_FD self, const char* varname, const char* value) {}
void RemoveVariable(PATRC_FD self, const char* varname) {}
void ModifyVariable(PATRC_FD self, const char* varname, const char* value) {}
void AddKey(PATRC_FD self, const char* block, const char* key, const char* value) {}
void RemoveKey(PATRC_FD self, const char* block, const char* key) {}
void ModifyKey(PATRC_FD self, const char* block, const char* key, const char* value) {}



bool ReadFiledata(PATRC_FD self, const char* path) {
    return Constructor(&self, path);
}

PATRC_FD CreateEmptyFiledata() {
    PATRC_FD res;
    if (!Constructor_Empty(&res)) return NULL;
    return res;
}
PATRC_FD CreateFiledata(const char* path) {
    PATRC_FD res;
    if (!Constructor(&res, path)) return NULL;
    return res;
}
void DestroyFiledata(PATRC_FD* self) {
    Destructor(self);
    *self = NULL;
}


int test(void) {
	return 111;
}