#include <ATRC.h>
#include <iostream>
#include <string>
#include <vector>
#include <filer.h>
/*+++
Wrap around functions from C++ to c
---*/

/*_ATRC_WRAP_READ*/
bool _ATRC_WRAP_FUNC_1(C_PATRC_FD self) {
    std::string filename = self->Filename;
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
    auto parsedData = atrc::ParseFile(filename, encoding, extension);
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
        return false;
    } else {
        for(atrc::Variable &var : *parsedData.first){
            self->Variables->Variables[self->Variables->VariableCount].Name = new char[var.Name.size() + 1];
            std::strcpy(self->Variables->Variables[self->Variables->VariableCount].Name, var.Name.c_str());
            self->Variables->Variables[self->Variables->VariableCount].Value = new char[var.Value.size() + 1];
            std::strcpy(self->Variables->Variables[self->Variables->VariableCount].Value, var.Value.c_str());
            self->Variables->Variables[self->Variables->VariableCount].IsPublic = var.IsPublic;
            self->Variables->VariableCount++;
        }
        for(atrc::Block &block : *parsedData.second){
            self->Blocks->Blocks[self->Blocks->BlockCount].Name = new char[block.Name.size() + 1];
            std::strcpy(self->Blocks->Blocks[self->Blocks->BlockCount].Name, block.Name.c_str());
            for(atrc::Key &key : block.Keys){
                self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Name = new char[key.Name.size() + 1];
                std::strcpy(self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Name, key.Name.c_str());
                self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Value = new char[key.Value.size() + 1];
                std::strcpy(self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Value, key.Value.c_str());
                self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount++;
            }
            self->Blocks->BlockCount++;
        }
    }
    return true;
}

/*_ATRC_WRAP_ERRORMSG*/
void _ATRC_WRAP_FUNC_2(int err_num, int line_number, const char *var_name, const char *filename){
    atrc::errormsg(err_num, line_number, var_name, filename);
}

/*_ATRC_WRAP__W_SAVE*/
void _ATRC_WRAP_FUNC_3(C_PATRC_FD self, const int action, const int xtra_info, const char *varname){
    atrc::ATRC_FD fd(self);
    atrc::_W_Save_(&fd, (atrc::ATRC_SAVE)action, xtra_info, varname);
}

/* INSERT_VAR */
void _ATRC_WRAP_FUNC_4(C_PATRC_FD self, const char* line, const char** args){
    atrc::ATRC_FD fd = atrc::ATRC_FD();
    std::vector<std::string> args_v;
    for(uint64_t i = 0; args[i] != NULL; i++){
        args_v.push_back(args[i]);
    }
    std::string line_s = line;
    fd.InsertVar(line_s, args_v);
}

/* INSERT_VAR_S */
const char* _ATRC_WRAP_FUNC_5(C_PATRC_FD self, const char* line, const char** args){
    atrc::ATRC_FD fd = atrc::ATRC_FD();
    std::vector<std::string> args_v;
    for(uint64_t i = 0; args[i] != NULL; i++){
        args_v.push_back(args[i]);
    }
    std::string line_s = line;
    std::string res = fd.InsertVar_S(line_s, args_v);
    return res.c_str();
}