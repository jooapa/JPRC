#ifndef FILER_H
#define FILER_H
#include <string>
#include <iostream>
#include <memory>
#include <vector>
// #ifdef __linux__
#   include <algorithm>
// #endif
#include "DEV_INC.hpp"




bool BlockContainsKey(std::vector<Key>& keys, const Key& key);
bool BlockContainsBlock(std::unique_ptr<std::vector<Block>>& blocks,const Block& block);
bool VariableContainsVariable(std::unique_ptr<std::vector<Variable>>& variables, const Variable& variable);
std::string ParseLineSTRINGtoATRC(const std::string &line);

enum class ATRC_SAVE{
    FULL_SAVE = -1, 
    ADD_BLOCK = 0, 
    REMOVE_BLOCK,
    ADD_KEY,
    REMOVE_KEY,
    MODIFY_KEY,
    ADD_VAR,
    REMOVE_VAR,
    MODIFY_VAR,
};

void Save(
std::shared_ptr<ATRC_FD> filedata, 
const ATRC_SAVE &action = ATRC_SAVE::FULL_SAVE, 
const int &xtra_info = -2, 
const std::string &xtra_info2 = "",
const std::string &xtra_info3 = "",
const std::string &xtra_info4 = ""
);

void _W_Save_(
PATRC_FD filedata,
const ATRC_SAVE &action = ATRC_SAVE::FULL_SAVE, 
const int &xtra_info = -2, 
const std::string &xtra_info2 = "",
const std::string &xtra_info3 = "",
const std::string &xtra_info4 = ""
);

// trim from start (in place)
inline void ltrim(std::string &s) {
s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
}));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

inline void errormsg(int err_num=-1, 
                    int line_number=-1, const 
                    std::string& var_name="",
                    std::string filename="no_filename"
                    ){
    std::string msg = "";
    int err_class = -1;
    switch(err_num){
        case ERR_STDLIB_CAST_ERROR:
            msg = "Unsuccesfull cast to other type in a STDLIB function: '" + var_name + "' at line " + std::to_string(line_number);
            err_class = ERR_CLASS_STDLIB;
            break;
        case ERR_INVALID_VAR_DECL:
            msg = "Invalid variable declaration: '" + var_name +"' at line " + std::to_string(line_number);
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_INVALID_BLOCK_DECL:
            msg = "Invalid block declaration at line " + std::to_string(line_number) + ". Stopping parsing";
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_INVALID_KEY_DECL:
            msg = "Invalid key declaration at line " + std::to_string(line_number);
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_NO_VAR_VECTOR:
            msg = "No variable vector found";
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_REREFERENCED_VAR:
            msg = "Re-Rereferenced variable: '" + var_name + "' at line " + std::to_string(line_number);
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_REREFERENCED_BLOCK:
            msg = "Re-referenced block: '" + var_name + "' at line " + std::to_string(line_number);
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_REREFERENCED_KEY:
            msg = "Re-Referenced key: '" + var_name + "' at line " + std::to_string(line_number);
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_INSERT_WRONG:
            msg = "Invalid insert variable declaration: '" + var_name + "'";
            err_class = ERR_INSERT_VAR;
            break;
        case ERR_UNAUTHORIZED_ACCESS_TO_VAR:
            msg = "Unauthorized access to variable: '" + var_name + "'";
            err_class = ERR_CLASS_READER;
            break;
        case ERR_BLOCK_NOT_FOUND:
            msg = "Block not found: '" + var_name + "'";
            err_class = ERR_CLASS_SAVER;
            break;
        case ERR_KEY_NOT_FOUND:
            msg = "Key not found: '" + var_name + "'";
            err_class = ERR_CLASS_SAVER;
            break;
        case ERR_VAR_NOT_FOUND:
            msg = "Variable not found: '" + var_name + "'";
            err_class = ERR_CLASS_SAVER;
            break;
        case ERR_BLOCK_EXISTS:
            msg = "Block already exists: '" + var_name + "'";
            err_class = ERR_CLASS_SAVER;
            break;
        case ERR_KEY_EXISTS:
            msg = "Key already exists: '" + var_name + "'";
            err_class = ERR_CLASS_SAVER;
            break;
        case ERR_VAR_EXISTS:
            msg = "Variable already exists: '" + var_name + "'";
            err_class = ERR_CLASS_SAVER;
            break;
        case ERR_INVALID_FILE:
            msg = "Error with filename or fileheader: '" + var_name + "'";
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        default:
            msg = "Unknown error at line " + std::to_string(line_number);
            break;
    }
    std::cerr << "ATRC Error<" << err_class << "?" << err_num << ">"<< "<" << filename << ">" <<": " << msg << std::endl;
}

std::pair<
    std::unique_ptr<std::vector<Variable>>, 
    std::unique_ptr<std::vector<Block>>
> 
ParseFile
(
    const std::string &filename, 
    const std::string &encoding, 
    const std::string &extension
    );

#endif // FILER_H