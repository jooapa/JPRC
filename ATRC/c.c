#define _CRT_SECURE_NO_DEPRECATE
#include "./include/ATRC.h"
#include "./include/filer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#error REDO c.c. MEMORY MANAGEMENT IS NOT PROPERLY HANDLED.

bool Read(PATRC_FD self, const char* path, ReadMode mode) {
    return _ATRC_WRAP_FUNC_1(self, path, mode);
}

uint64_t GetEnumValue(PATRC_FD self, const char* block, const char* key) {
    for(uint64_t i = 0; i < self->Blocks->BlockCount; i++){
        if(strcmp(block, self->Blocks->Blocks[i].Name) == 0){
            for(uint64_t j = 0; j < self->Blocks->Blocks[i].KeyCount; j++){
                if(strcmp(key, self->Blocks->Blocks[i].Keys[j].Name) == 0){
                    return self->Blocks->Blocks[i].Keys[j].enum_value;
                }
            }
        }
    }
    return (uint64_t)-1; // Return -1 if not found
}

const char* ReadVariable(PATRC_FD self, const char* varname) {
    const char* res = "";
    for(uint64_t i = 0; i < self->Variables->VariableCount; i++){
        if (self->Variables->Variables[i].Name == NULL) return NULL;
        if(strcmp(varname, self->Variables->Variables[i].Name) == 0){
            if(self->Variables->Variables[i].IsPublic) {
				if (self->Variables->Variables[i].Value == NULL) return NULL;
                res = self->Variables->Variables[i].Value;
            } else {
                _ATRC_WRAP_FUNC_2(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, self->Filename);
                res = NULL;
            }
        }
    }
    return res;
}
const char* ReadKey(PATRC_FD self, const char* block, const char* key) {
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
    if(strcmp(res, "") == 0) return NULL;
    return res;
}
bool DoesExistBlock(PATRC_FD self, const char* block) {
    for(uint64_t i = 0; i < self->Blocks->BlockCount; i++){
        if(strcmp(block, self->Blocks->Blocks[i].Name) == 0) return true;
    }
    return false;
}
bool DoesExistVariable(PATRC_FD self, const char* varname) {
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
bool DoesExistKey(PATRC_FD self, const char* block, const char* key) {
    for(uint64_t i = 0; i < self->Blocks->BlockCount; i++){
        if(strcmp(block, self->Blocks->Blocks[i].Name) == 0){
            for(uint64_t j = 0; j < self->Blocks->Blocks[i].KeyCount; j++){
                if(strcmp(key, self->Blocks->Blocks[i].Keys[j].Name) == 0) return true;
            }
        }
    }
    return false;
}
bool IsPublic(PATRC_FD self, const char* varname) {
    for(uint64_t i = 0; i < self->Variables->VariableCount; i++){
        if(strcmp(varname, self->Variables->Variables[i].Name) == 0) return self->Variables->Variables[i].IsPublic;
    }
    return false;
}
void InsertVar(char* line, const char** args) {
    _ATRC_WRAP_FUNC_4(line, args);
}
char* InsertVar_S(const char* line, const char** args) {
    return _ATRC_WRAP_FUNC_5(line, args);
}
bool AddBlock(PATRC_FD self, const char* blockname) {
    if (DoesExistBlock(self, blockname)) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_EXISTS, -1, blockname, self->Filename);
        return false;
    }

    Block blk;
    blk.Name = (char*)malloc(strlen(blockname) + 1);
    if (blk.Name == NULL) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, blockname, self->Filename);
        return false;
    }
    strcpy(blk.Name, blockname);
    blk.Keys = NULL;
    blk.KeyCount = 0;
    blk.line_number = (uint64_t) - 1;

    // Expand Blocks array if 
	Block* newBlocks = (Block*)realloc(self->Blocks->Blocks, (self->Blocks->BlockCount + 1) * sizeof(Block));
	if (newBlocks == NULL) {
		free(blk.Name);
		free(blk.Keys);
		_ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, blockname, self->Filename);
		return false;
	}
	self->Blocks->Blocks = newBlocks;
    if (self->Blocks->Blocks == NULL) {
        free(blk.Name);
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

bool RemoveBlock(PATRC_FD self, const char* blockname) {
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

    if (index == (size_t)-1) {
        _ATRC_WRAP_FUNC_2(ERR_BLOCK_NOT_FOUND, -1, blockname, self->Filename);
        return false;
    }

    // Free block contents
    free(self->Blocks->Blocks[index].Name);
    self->Blocks->Blocks[index].Name = NULL;

    for (size_t j = 0; j < self->Blocks->Blocks[index].KeyCount; j++) {
        free(self->Blocks->Blocks[index].Keys[j].Name);
		self->Blocks->Blocks[index].Keys[j].Name = NULL;
        free(self->Blocks->Blocks[index].Keys[j].Value);
		self->Blocks->Blocks[index].Keys[j].Value = NULL;
    }
    free(self->Blocks->Blocks[index].Keys);
    self->Blocks->Blocks[index].Keys = NULL;

    // Shift remaining blocks
    for (size_t i = index; i < self->Blocks->BlockCount - 1; i++) {
        self->Blocks->Blocks[i] = self->Blocks->Blocks[i + 1];
    }

    // Reduce block count and reallocate memory
    self->Blocks->BlockCount--;
	Block* newBlocks = (Block*)realloc(self->Blocks->Blocks, self->Blocks->BlockCount * sizeof(Block));
	if (newBlocks == NULL) {
		_ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, blockname, self->Filename);
		return false;
	}
	self->Blocks->Blocks = newBlocks;

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, REMOVE_BLOCK, (int)index, blockname, "", "");
    }
    return true;
}

bool AddVariable(PATRC_FD self, const char* varname, const char* value) {
    if (DoesExistVariable(self, varname)) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
        return false;
    }

    Variable var;
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
	Variable* newVars = (Variable*)realloc(self->Variables->Variables, (self->Variables->VariableCount + 1) * sizeof(Variable));
	if (newVars == NULL) {
		free(var.Name);
		free(var.Value);
		_ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
		return false;
	}
	self->Variables->Variables = newVars;
    
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

bool RemoveVariable(PATRC_FD self, const char* varname) {
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

    if (index == (size_t)-1) return false; // Should not happen due to DoesExistVariable

    // Free variable contents
    free(self->Variables->Variables[index].Name);
    free(self->Variables->Variables[index].Value);

    // Shift remaining variables
    for (size_t i = index; i < self->Variables->VariableCount - 1; i++) {
        self->Variables->Variables[i] = self->Variables->Variables[i + 1];
    }

    // Reduce variable count and reallocate memory
    self->Variables->VariableCount--;
	Variable* newVars = (Variable*)realloc(self->Variables->Variables, self->Variables->VariableCount * sizeof(Variable));
	if (newVars == NULL) {
		_ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
		return false;
	}
	self->Variables->Variables = newVars;

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, REMOVE_VAR, (int)index, varname, "", "");
    }
    return true;
}

bool ModifyVariable(PATRC_FD self, const char* varname, const char* value) {
    if (!DoesExistVariable(self, varname)) {
        if(!self->Writecheck){
            _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, varname, self->Filename);
            return false;
        } else {
            if(!AddVariable(self, varname, value)){
                _ATRC_WRAP_FUNC_2(ERR_WRITECHECK, -1, varname, self->Filename);
                return false;
            }
        }
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
                _ATRC_WRAP_FUNC_3(self, MODIFY_VAR, (int)i, value, "", "");
            }
            return true;
        }
    }
	return false;
}

bool AddKey(PATRC_FD self, const char* block, const char* key, const char* value) {
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

    if (blockIndex == (size_t)-1) return false;

    Block* blk = &self->Blocks->Blocks[blockIndex];
    if (DoesExistKey(self, block, key)) {
        _ATRC_WRAP_FUNC_2(ERR_KEY_EXISTS, -1, key, block);
        return false;
    }

    // Create the key
    Key newKey;
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
	Key* newKeys = (Key*)realloc(blk->Keys, (blk->KeyCount + 1) * sizeof(Key));
	if (newKeys == NULL) {
		free(newKey.Name);
		free(newKey.Value);
		_ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, block);
		return false;
	}
	blk->Keys = newKeys;
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

bool RemoveKey(PATRC_FD self, const char* block, const char* key) {
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
    if (blockIndex == (size_t)-1) return false;

    Block* blk = &self->Blocks->Blocks[blockIndex];
    for (size_t i = 0; i < blk->KeyCount; i++) {
        if (strcmp(blk->Keys[i].Name, key) == 0) {
            keyIndex = i;
            break;
        }
    }
    if (keyIndex == (size_t)-1) return false;

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
        Key* newKeys = (Key*)realloc(blk->Keys, blk->KeyCount * sizeof(Key));
        if (newKeys == NULL) {
            _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, self->Filename);
            return false;
        }
        blk->Keys = newKeys;
    }

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, REMOVE_KEY, (int)keyIndex, key, "", block);
    }
    return true;
}

bool ModifyKey(PATRC_FD self, const char* block, const char* key, const char* value) {
    if (!DoesExistBlock(self, block)) {
        if(!self->Writecheck){
            _ATRC_WRAP_FUNC_2(ERR_BLOCK_NOT_FOUND, -1, block, self->Filename);
            return false;
        } else {
            if(!AddBlock(self, block)){
                _ATRC_WRAP_FUNC_2(ERR_WRITECHECK, -1, block, self->Filename);
                return false;
            }
        }
    }
    if (!DoesExistKey(self, block, key)) {
        if(!self->Writecheck){
            _ATRC_WRAP_FUNC_2(ERR_KEY_NOT_FOUND, -1, key, self->Filename);
            return false;
        } else {
            if(!AddKey(self, block, key, value)){
                _ATRC_WRAP_FUNC_2(ERR_WRITECHECK, -1, key, self->Filename);
                return false;
            }
        }
    }

    size_t blockIndex = -1, keyIndex = -1;
    for (size_t i = 0; i < self->Blocks->BlockCount; i++) {
        if (strcmp(self->Blocks->Blocks[i].Name, block) == 0) {
            blockIndex = i;
            break;
        }
    }
    if (blockIndex == (size_t)-1) return false;

    Block* blk = &self->Blocks->Blocks[blockIndex];
    for (size_t i = 0; i < blk->KeyCount; i++) {
        if (strcmp(blk->Keys[i].Name, key) == 0) {
            keyIndex = i;
            break;
        }
    }
    if (keyIndex == (size_t)-1) return false;

    // Update the key's value
    free(blk->Keys[keyIndex].Value);
    blk->Keys[keyIndex].Value = (char*)malloc(strlen(value) + 1);
    if (blk->Keys[keyIndex].Value == NULL) {
        _ATRC_WRAP_FUNC_2(ERR_MEMORY_ALLOCATION_FAILED, -1, key, self->Filename);
        return false;
    }
    strcpy(blk->Keys[keyIndex].Value, value);

    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, MODIFY_KEY, (int)keyIndex, key, value, block);
    }
    return true;
}

bool WriteCommentToBottom(PATRC_FD self, const char* comment) {
    if (!self) return false;

    // Add the comment line to the end of the file
    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, WRITE_COMMENT_TO_BOTTOM, -1, comment, "", "");
    }
    return true;
}
bool WriteCommentToTop(PATRC_FD self, const char* comment) {
    if (!self) return false;

    // Add the comment line to the top of the file
    if (self->AutoSave) {
        _ATRC_WRAP_FUNC_3(self, WRITE_COMMENT_TO_TOP, -1, comment, "", "");
    }
    return true;
}

PATRC_FD Create_ATRC_FD(const char *filename, ReadMode mode){
    PATRC_FD res = Create_Empty_ATRC_FD();
    printf("Creating ATRC_FD for file: %s\n", filename);
    if(res == NULL) return NULL;

    if(res->Filename != NULL) {
        free(res->Filename);
    }
    res->Filename = (char*)malloc(strlen(filename) + 1);
    if(res->Filename == NULL) {
        free(res);
        return NULL;
    }
    strcpy(res->Filename, filename);
    if(!Read(res, filename, mode)) return NULL;
    return res;
}
PATRC_FD Create_Empty_ATRC_FD(void){
    PATRC_FD res = (PATRC_FD)malloc(sizeof(ATRC_FD));
    if(res == NULL) return NULL;
    res->AutoSave = false;
    res->Writecheck = false;
    res->Variables = (PVariable_Arr)malloc(sizeof(Variable_Arr));
	if (res->Variables == NULL) {
		free(res);
		return NULL;
	}
	res->Variables->Variables = NULL;
	res->Variables->VariableCount = 0;
    res->Blocks = (PBlock_Arr)malloc(sizeof(Block_Arr));
	if (res->Blocks == NULL) {
		free(res->Variables);
		free(res);
		return NULL;
	}
	res->Blocks->Blocks = NULL;
	res->Blocks->BlockCount = 0;

    res->Filename = NULL;
    return res;
}

void *Destroy_ATRC_FD_Variables(PATRC_FD self) {
	if (!self) return self;
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
        free(self->Variables);
        self->Variables = NULL;
    }
    return self;
}
void *Destroy_ATRC_FD_Blocks_And_Keys(PATRC_FD self) {
	if (!self) return self;
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
        free(self->Blocks);
        self->Blocks = NULL;
    }
    return self;
}

void *Destroy_ATRC_FD(PATRC_FD self) {
    if (!self) return self;
	if(!Destroy_ATRC_FD_Blocks_And_Keys(self)) return self;
	if(!Destroy_ATRC_FD_Variables(self)) return self;
    // Free Filename
    if (self->Filename) {
        free(self->Filename);
        self->Filename = NULL;
    }

    // Free the main structure
    free(self);
    self = NULL;
    return self;
}


