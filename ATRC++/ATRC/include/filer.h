#ifndef FILER_H
#define FILER_H
#include <string>
#include <iostream>
#include <memory>
#include <vector>
// #ifdef __linux__
#   include <algorithm>
// #endif
#include "ATRC.hpp"


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

void Save(ATRC_FD *filedata, 
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

inline std::string str_to_lower(const std::string &str){
    std::string res=str;
    for(char &c : res){
        std::tolower(c);
    }
    return res;
}

#define ERR_CLASS_FILEHANDLER           100
#define ERR_INVALID_VAR_DECL            101
#define ERR_INVALID_BLOCK_DECL          102
#define ERR_INVALID_KEY_DECL            103
#define ERR_NO_VAR_VECTOR               104
#define ERR_REREFERENCED_VAR            105
#define ERR_REREFERENCED_BLOCK          106
#define ERR_REREFERENCED_KEY            107
#define ERR_INSERT_VAR                  108  
#define ERR_INVALID_FILE                109
#define ERR_CLASS_STDLIB                110

#define ERR_CLASS_READER                200
#define ERR_UNAUTHORIZED_ACCESS_TO_VAR  201

#define ERR_CLASS_SAVER                 300
#define ERR_BLOCK_NOT_FOUND             301
#define ERR_KEY_NOT_FOUND               302
#define ERR_VAR_NOT_FOUND               303
#define ERR_BLOCK_EXISTS                304
#define ERR_KEY_EXISTS                  305
#define ERR_VAR_EXISTS                  306
#define ERR_INSERT_WRONG                307
#define ERR_STDLIB_CAST_ERROR           308

inline void errormsg(int err_num=-1, 
                    int line_number=-1, const 
                    std::string& var_name="",
                    std::string filename="no_filename"
                    ){
    std::string msg = "";
    int err_class = -1;
    switch(err_num){
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
            msg = "Error with filename or its extension: '" + var_name + "'";
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        default:
            msg = "Unknown error at line " + std::to_string(line_number);
            break;
    }
    std::cerr << "ATRC Error<" << err_class << "?" << err_num << ">"<< "<" << filename << ">" <<": " << msg << std::endl;
}

#endif // FILER_H