#define _CRT_SECURE_NO_WARNINGS
#include "./include/ATRC.h"
#include "./include/filer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <cstdlib>
/*+++
Wrap around functions from C++ to c
---*/

/*_ATRC_WRAP_READ*/
bool _ATRC_WRAP_FUNC_1(PATRC_FD self, const char* path, ReadMode readMode) {
    std::string filename = path;
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
        if (readMode == ATRC_FORCE_READ) {
        // Delete previous file and create a new one
        std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
        if (ofs.is_open()) {
            ofs << "#!ATRC" << "\n";
        } else {
            errormsg(FILE_MODE_ERR, -1, filename, filename);
            return false;
        }
    } else if (readMode == ATRC_CREATE_READ) {
        std::ifstream ifs(filename);
        if (!ifs.good()) {
            std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
            if (ofs.is_open()) {
                ofs << "#!ATRC" << "\n";
            } else {
                errormsg(FILE_MODE_ERR, -1, filename, filename);
                return false;
            }
        }
    }
    std::vector<CPP_Variable> variables;
    std::vector<CPP_Block> blocks;
    auto parsedData = ParseFile(filename, encoding, extension, variables, blocks);
    #if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
        self->Filename = _strdup(filename.c_str());
    #else
        self->Filename = strdup(filename.c_str());
    #endif
    if (!parsedData) {
        errormsg(ERR_INVALID_FILE, -1, filename, filename);
        return false;
    }

    // Cleanup existing data
    Destroy_ATRC_FD_Blocks_And_Keys(self);
    Destroy_ATRC_FD_Variables(self);

    bool old_autoSave = self->AutoSave;
    bool old_writecheck = self->Writecheck;

    // Allocate main structures
    self->Variables = (PVariable_Arr)malloc(sizeof(Variable_Arr));
    self->Blocks = (PBlock_Arr)malloc(sizeof(Block_Arr));
    if (!self->Variables || !self->Blocks) {
        Destroy_ATRC_FD(self);
        return false;
    }
    self->Variables->Variables = nullptr;
    self->Blocks->Blocks = nullptr;
    self->Variables->VariableCount = 0;
    self->Blocks->BlockCount = 0;
    self->AutoSave = old_autoSave;
    self->Writecheck = old_writecheck;
    // Process variables
    self->Variables->VariableCount = variables.size();
    self->Variables->Variables = (PVariable)malloc(self->Variables->VariableCount * sizeof(Variable));
    if (!self->Variables->Variables) {
        Destroy_ATRC_FD(self);
        return false;
    }

    for (size_t i = 0; i < variables.size(); i++) {
        const auto &var = variables[i];
        #if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
        self->Variables->Variables[i].Name = _strdup(var.Name.c_str());
        self->Variables->Variables[i].Value = _strdup(var.Value.c_str());
        #else
        self->Variables->Variables[i].Name = strdup(var.Name.c_str());
        self->Variables->Variables[i].Value = strdup(var.Value.c_str());
        #endif
        self->Variables->Variables[i].line_number = var.line_number;
        self->Variables->Variables[i].IsPublic = var.IsPublic;
    }

    // Process blocks
    self->Blocks->BlockCount = blocks.size();
    self->Blocks->Blocks = (PBlock)malloc(self->Blocks->BlockCount * sizeof(Block));
    if (!self->Blocks->Blocks) {
        Destroy_ATRC_FD(self);
        return false;
    }

    for (size_t i = 0; i < blocks.size(); i++) {
        const auto &block = blocks[i];
        #if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
        self->Blocks->Blocks[i].Name = _strdup(block.Name.c_str());
        #else
        self->Blocks->Blocks[i].Name = strdup(block.Name.c_str());
        #endif
        self->Blocks->Blocks[i].line_number = block.line_number;
        self->Blocks->Blocks[i].KeyCount = block.Keys.size();
        self->Blocks->Blocks[i].Keys = (PKey)malloc(self->Blocks->Blocks[i].KeyCount * sizeof(Key));

        if (!self->Blocks->Blocks[i].Keys) {
            Destroy_ATRC_FD(self);
            return false;
        }

        for (size_t j = 0; j < block.Keys.size(); j++) {
            const auto &key = block.Keys[j];
            #if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
            self->Blocks->Blocks[i].Keys[j].Name = _strdup(key.Name.c_str());
            self->Blocks->Blocks[i].Keys[j].Value = _strdup(key.Value.c_str());
            #else
            self->Blocks->Blocks[i].Keys[j].Name = strdup(key.Name.c_str());
            self->Blocks->Blocks[i].Keys[j].Value = strdup(key.Value.c_str());
            #endif
            self->Blocks->Blocks[i].Keys[j].line_number = key.line_number;
            self->Blocks->Blocks[i].Keys[j].enum_value = key.enum_value;
        }
    }

    return true;
}

/*_ATRC_WRAP_ERRORMSG*/
void _ATRC_WRAP_FUNC_2(int err_num, int line_number, const char *var_name, const char *filename){
    errormsg(err_num, line_number, var_name, filename);
}

/*_ATRC_WRAP__W_SAVE*/
void _ATRC_WRAP_FUNC_3(
    PATRC_FD self, 
    const int action, 
    const int xtra_info, 
    const char *varname, 
    const char *xtra_info4,
    const char *xtra_info5
){
    CPP_ATRC_FD fd(self);

    if((ATRC_SAVE)action == ATRC_SAVE::ADD_VAR){
        std::string temp1 = varname;
        std::string temp2 = xtra_info4;
        _W_Save_(&fd, (ATRC_SAVE)action, xtra_info, "%"+temp1+"%="+ParseLineSTRINGtoATRC(temp2));
    } 
    else if((ATRC_SAVE)action == ATRC_SAVE::MODIFY_VAR){
        std::string temp1 = varname;
        _W_Save_(&fd, (ATRC_SAVE)action, xtra_info, ParseLineSTRINGtoATRC(varname));
    }
    else {
        _W_Save_(&fd, (ATRC_SAVE)action, xtra_info, varname, xtra_info4, xtra_info5);
    }
}

/* INSERT_VAR */
void _ATRC_WRAP_FUNC_4(char* line, const char** args){
    CPP_ATRC_FD fd = CPP_ATRC_FD();
    std::vector<std::string> args_v;
    for(uint64_t i = 0; args[i] != NULL; i++){
        args_v.push_back(args[i]);
    }
    std::string line_s = line;
    fd.InsertVar(line_s, args_v);
    size_t line_size = strlen(line);
    strncpy(line, line_s.c_str(), line_size - 1);
    line[line_size - 1] = '\0';
}

/* INSERT_VAR_S */
char* _ATRC_WRAP_FUNC_5(const char* line, const char** args) {
    CPP_ATRC_FD fd = CPP_ATRC_FD();
    std::vector<std::string> args_v;
    for (uint64_t i = 0; args[i] != NULL; i++) {
        args_v.push_back(args[i]);
    }
    std::string line_s = line;
    std::string res = fd.InsertVar_S(line_s, args_v);

    if (res.empty()) {
        return nullptr;
    }

    char *cstr = (char*)malloc(res.length() + 1);
    if (cstr == NULL) {
        return nullptr; // Memory allocation failed
    }
    strcpy(cstr, res.c_str());
    cstr[res.length()] = '\0'; // Null-terminate the string
    
    return cstr;
}

void *ATRC_FREE_MEMORY(void *ptr){
    if (ptr != NULL) {
        free(ptr);
		ptr = NULL;
    }
    return NULL;
}