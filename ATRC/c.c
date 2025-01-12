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
    return _ATRC_WRAP_FUNC_4(self, line, args);
}
const char* InsertVar_S(C_PATRC_FD self, const char* line, const char** args) {
    return _ATRC_WRAP_FUNC_5(self, line, args);
}
bool AddBlock(C_PATRC_FD self, const char* blockname) {
    if (DoesExistBlock(self, blockname)) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_EXISTS, -1, blockname, self->Filename);
        return false;
    }

    C_Block blk;
    blk.Name = (char*)malloc(strlen(blockname) + 1);
    if (blk.Name == NULL) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, blockname, self->Filename);
        return false;
    }
    strcpy(blk.Name, blockname);

    blk.Keys = (C_Key*)malloc(sizeof(C_Key)); // Allocate for the keys
    if (blk.Keys == NULL) {
        free(blk.Name);
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, blockname, self->Filename);
        return false;
    }
    blk.KeyCount = 0;

    // Expand Blocks array if necessary
    self->Blocks->Blocks = (C_Block*)realloc(self->Blocks->Blocks, (self->Blocks->BlockCount + 1) * sizeof(C_Block));
    if (self->Blocks->Blocks == NULL) {
        free(blk.Name);
        free(blk.Keys);
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, blockname, self->Filename);
        return false;
    }

    self->Blocks->Blocks[self->Blocks->BlockCount] = blk;
    self->Blocks->BlockCount++;

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, ADD_BLOCK, -1, blockname, "", "");
    }
    return true;
}

bool RemoveBlock(C_PATRC_FD self, const char* blockname) {
    if (!DoesExistBlock(self, blockname)) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_NOT_FOUND, -1, blockname, self->Filename);
        return false;
    }

    // Find the block index
    size_t index = -1;
    for (size_t i = 0; i < self->Blocks->BlockCount; i++) {
        if (strcmp(self->Blocks->Blocks[i].Name, blockname) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_NOT_FOUND, -1, blockname, self->Filename);
        return false;
    }

    // Free block contents
    free(self->Blocks->Blocks[index].Name);
    self->Blocks->Blocks[index].Name = NULL;

    for (size_t j = 0; j < self->Blocks->Blocks[index].KeyCount; j++) {
        free(self->Blocks->Blocks[index].Keys[j].Name);
        free(self->Blocks->Blocks[index].Keys[j].Value);
    }
    free(self->Blocks->Blocks[index].Keys);
    self->Blocks->Blocks[index].Keys = NULL;

    // Shift remaining blocks
    for (size_t i = index; i < self->Blocks->BlockCount - 1; i++) {
        self->Blocks->Blocks[i] = self->Blocks->Blocks[i + 1];
    }

    // Reduce block count and reallocate memory
    self->Blocks->BlockCount--;
    self->Blocks->Blocks = (C_Block*)realloc(self->Blocks->Blocks, self->Blocks->BlockCount * sizeof(C_Block));

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, REMOVE_BLOCK, index, blockname, "", "");
    }
    return true;
}

bool AddVariable(C_PATRC_FD self, const char* varname, const char* value) {
    if (DoesExistVariable(self, varname)) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
        return false;
    }

    C_Variable var;
    var.Name = (char*)malloc(strlen(varname) + 1);
    if (var.Name == NULL) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
        return false;
    }
    strcpy(var.Name, varname);

    var.Value = (char*)malloc(strlen(value) + 1);
    if (var.Value == NULL) {
        free(var.Name);
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
        return false;
    }
    strcpy(var.Value, value);

    var.IsPublic = true;

    // Expand Variables array if necessary
    self->Variables->Variables = (C_Variable*)realloc(self->Variables->Variables, (self->Variables->VariableCount + 1) * sizeof(C_Variable));
    if (self->Variables->Variables == NULL) {
        free(var.Name);
        free(var.Value);
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
        return false;
    }

    self->Variables->Variables[self->Variables->VariableCount] = var;
    self->Variables->VariableCount++;

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, ADD_VAR, -1, varname, value, "");
    }
    return true;
}

bool RemoveVariable(C_PATRC_FD self, const char* varname) {
    if (!DoesExistVariable(self, varname)) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
        return false;
    }

    // Find the variable index
    size_t index = -1;
    for (size_t i = 0; i < self->Variables->VariableCount; i++) {
        if (strcmp(self->Variables->Variables[i].Name, varname) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) return false; // Should not happen due to DoesExistVariable

    // Free variable contents
    free(self->Variables->Variables[index].Name);
    free(self->Variables->Variables[index].Value);

    // Shift remaining variables
    for (size_t i = index; i < self->Variables->VariableCount - 1; i++) {
        self->Variables->Variables[i] = self->Variables->Variables[i + 1];
    }

    // Reduce variable count and reallocate memory
    self->Variables->VariableCount--;
    self->Variables->Variables = (C_Variable*)realloc(self->Variables->Variables, self->Variables->VariableCount * sizeof(C_Variable));

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, REMOVE_VAR, index, varname, "", "");
    }
    return true;
}

bool ModifyVariable(C_PATRC_FD self, const char* varname, const char* value) {
    if (!DoesExistVariable(self, varname)) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
        return false;
    }

    // Find the variable
    for (size_t i = 0; i < self->Variables->VariableCount; i++) {
        if (strcmp(self->Variables->Variables[i].Name, varname) == 0) {
            // Modify the value
            free(self->Variables->Variables[i].Value);
            self->Variables->Variables[i].Value = (char*)malloc(strlen(value) + 1);
            if (self->Variables->Variables[i].Value == NULL) {
                _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
                return false;
            }
            strcpy(self->Variables->Variables[i].Value, value);

            if (self->AutoSave) {
                _ATRC_WRAP_FUNC_3(self, MODIFY_VAR, i, value, "", "");
            }
            return true;
        }
    }
}

bool AddKey(C_PATRC_FD self, const char* block, const char* key, const char* value) {
    if (!DoesExistBlock(self, block)) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_NOT_FOUND, -1, block, self->Filename);
        return false;
    }

    // Find the block
    size_t blockIndex = -1;
    for (size_t i = 0; i < self->Blocks->BlockCount; i++) {
        if (strcmp(self->Blocks->Blocks[i].Name, block) == 0) {
            blockIndex = i;
            break;
        }
    }

    if (blockIndex == -1) return false;

    C_Block* blk = &self->Blocks->Blocks[blockIndex];
    if (DoesExistKey(self, block, key)) {
        _ATRC_WRAP_FUNC_2(ERR_KEY_EXISTS, -1, key, block);
        return false;
    }

    // Create the key
    C_Key newKey;
    newKey.Name = (char*)malloc(strlen(key) + 1);
    if (newKey.Name == NULL) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, block);
        return false;
    }
    strcpy(newKey.Name, key);

    newKey.Value = (char*)malloc(strlen(value) + 1);
    if (newKey.Value == NULL) {
        free(newKey.Name);
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, block);
        return false;
    }
    strcpy(newKey.Value, value);

    // Add the key to the block
    blk->Keys = (C_Key*)realloc(blk->Keys, (blk->KeyCount + 1) * sizeof(C_Key));
    if (blk->Keys == NULL) {
        free(newKey.Name);
        free(newKey.Value);
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, block);
        return false;
    }

    blk->Keys[blk->KeyCount] = newKey;
    blk->KeyCount++;

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, ADD_KEY, -1, key, value, block);
    }
    return true;
}

bool RemoveKey(C_PATRC_FD self, const char* block, const char* key) {
    if (!DoesExistBlock(self, block)) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_NOT_FOUND, -1, block, self->Filename);
        return false;
    }
    if (!DoesExistKey(self, block, key)) {
        _ATRC_WRAP_FUNC_2(ERR_KEY_NOT_FOUND, -1, key, self->Filename);
        return false;
    }

    size_t blockIndex = -1, keyIndex = -1;
    for (size_t i = 0; i < self->Blocks->BlockCount; i++) {
        if (strcmp(self->Blocks->Blocks[i].Name, block) == 0) {
            blockIndex = i;
            break;
        }
    }
    if (blockIndex == -1) return false;

    C_Block* blk = &self->Blocks->Blocks[blockIndex];
    for (size_t i = 0; i < blk->KeyCount; i++) {
        if (strcmp(blk->Keys[i].Name, key) == 0) {
            keyIndex = i;
            break;
        }
    }
    if (keyIndex == -1) return false;

    // Free key memory
    free(blk->Keys[keyIndex].Name);
    free(blk->Keys[keyIndex].Value);

    // Shift keys to fill the gap
    for (size_t i = keyIndex; i < blk->KeyCount - 1; i++) {
        blk->Keys[i] = blk->Keys[i + 1];
    }

    // Reduce key count and reallocate memory
    blk->KeyCount--;
    if (blk->KeyCount == 0) {
        free(blk->Keys);
        blk->Keys = NULL;
    } else {
        C_Key* newKeys = (C_Key*)realloc(blk->Keys, blk->KeyCount * sizeof(C_Key));
        if (newKeys == NULL) {
            _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, self->Filename);
            return false;
        }
        blk->Keys = newKeys;
    }

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, REMOVE_KEY, keyIndex, key, "", block);
    }
    return true;
}

bool ModifyKey(C_PATRC_FD self, const char* block, const char* key, const char* value) {
    if (!DoesExistBlock(self, block)) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_NOT_FOUND, -1, block, self->Filename);
        return false;
    }
    if (!DoesExistKey(self, block, key)) {
        _ATRC_WRAP_FUNC_2(ERR_KEY_NOT_FOUND, -1, key, self->Filename);
        return false;
    }

    size_t blockIndex = -1, keyIndex = -1;
    for (size_t i = 0; i < self->Blocks->BlockCount; i++) {
        if (strcmp(self->Blocks->Blocks[i].Name, block) == 0) {
            blockIndex = i;
            break;
        }
    }
    if (blockIndex == -1) return false;

    C_Block* blk = &self->Blocks->Blocks[blockIndex];
    for (size_t i = 0; i < blk->KeyCount; i++) {
        if (strcmp(blk->Keys[i].Name, key) == 0) {
            keyIndex = i;
            break;
        }
    }
    if (keyIndex == -1) return false;

    // Update the key's value
    free(blk->Keys[keyIndex].Value);
    blk->Keys[keyIndex].Value = (char*)malloc(strlen(value) + 1);
    if (blk->Keys[keyIndex].Value == NULL) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, self->Filename);
        return false;
    }
    strcpy(blk->Keys[keyIndex].Value, value);

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, MODIFY_KEY, keyIndex, key, value, block);
    }
    return true;
}

C_PATRC_FD Create_ATRC_FD(char *filename){
    C_PATRC_FD res = Create_Empty_ATRC_FD();
    if(res == NULL) return NULL;
    strcpy(res->Filename, filename);
    if(!Read(res)) return NULL;
    return res;
}
C_PATRC_FD Create_Empty_ATRC_FD(void){
    C_PATRC_FD res = (C_PATRC_FD)malloc(sizeof(C_ATRC_FD));
    if(res == NULL) return NULL;
    res->AutoSave = false;
    res->Variables = (C_PVariable_Arr)malloc(sizeof(C_Variable_Arr));
    res->Blocks = (C_PBlock_Arr)malloc(sizeof(C_Block_Arr));
    res->Filename = NULL;
    return res;
}


void Destroy_ATRC_FD(C_PATRC_FD self) {
    if (!self) return;

    // Free Variables
    if (self->Variables) {
        if (self->Variables->Variables) {
            for (uint64_t i = 0; i < self->Variables->VariableCount; i++) {
                if (self->Variables->Variables[i].Name) {
                    free(self->Variables->Variables[i].Name);
                    self->Variables->Variables[i].Name = NULL;
                }
                if (self->Variables->Variables[i].Value) {
                    free(self->Variables->Variables[i].Value);
                    self->Variables->Variables[i].Value = NULL;
                }
            }
            free(self->Variables->Variables);
            self->Variables->Variables = NULL;
        }
        self->Variables->VariableCount = 0;
        free(self->Variables);
        self->Variables = NULL;
    }

    // Free Blocks
    if (self->Blocks) {
        if (self->Blocks->Blocks) {
            for (uint64_t i = 0; i < self->Blocks->BlockCount; i++) {
                if (self->Blocks->Blocks[i].Name) {
                    free(self->Blocks->Blocks[i].Name);
                    self->Blocks->Blocks[i].Name = NULL;
                }
                if (self->Blocks->Blocks[i].Keys) {
                    for (uint64_t j = 0; j < self->Blocks->Blocks[i].KeyCount; j++) {
                        if (self->Blocks->Blocks[i].Keys[j].Name) {
                            free(self->Blocks->Blocks[i].Keys[j].Name);
                            self->Blocks->Blocks[i].Keys[j].Name = NULL;
                        }
                        if (self->Blocks->Blocks[i].Keys[j].Value) {
                            free(self->Blocks->Blocks[i].Keys[j].Value);
                            self->Blocks->Blocks[i].Keys[j].Value = NULL;
                        }
                    }
                    free(self->Blocks->Blocks[i].Keys);
                    self->Blocks->Blocks[i].Keys = NULL;
                }
            }
            free(self->Blocks->Blocks);
            self->Blocks->Blocks = NULL;
        }
        self->Blocks->BlockCount = 0;
        free(self->Blocks);
        self->Blocks = NULL;
    }

    // Free Filename
    if (self->Filename) {
        free(self->Filename);
        self->Filename = NULL;
    }

    // Free the main structure
    free(self);
}

