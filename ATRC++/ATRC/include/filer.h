#ifndef FILER_H
#define FILER_H
#include <string>
#include <iostream>
#include <vector>
#include "ATRC.h"


bool BlockContainsKey(std::vector<Key>* keys, Key* key);
bool BlockContainsBlock(std::vector<Block>* blocks, Block* block);
bool VariableContainsVariable(std::vector<Variable>* variables, Variable* variable);

#define AUTOSAVE_ADD_BLOCK          0
#define AUTOSAVE_REMOVE_BLOCK       1
#define AUTOSAVE_ADD_KEY            2
#define AUTOSAVE_REMOVE_KEY         3
#define AUTOSAVE_MODIFY_KEY         4
#define AUTOSAVE_ADD_VAR            5
#define AUTOSAVE_REMOVE_VAR         6
#define AUTOSAVE_MODIFY_VAR         7

void Save(ATRCFiledata *filedata, int action);

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

#define ERR_CLASS_FILEHANDLER           100
#define ERR_INVALID_VAR_DECL            101
#define ERR_INVALID_BLOCK_DECL          102
#define ERR_INVALID_KEY_DECL            103
#define ERR_NO_VAR_VECTOR               104
#define ERR_REREFERENCED_VAR            105
#define ERR_REREFERENCED_BLOCK          106
#define ERR_REREFERENCED_KEY            107  

#define ERR_CLASS_READER                200
#define ERR_UNAUTHORIZED_ACCESS_TO_VAR  201

#define ERR_CLASS_SAVER                 300
#define ERR_BLOCK_NOT_FOUND             301
#define ERR_KEY_NOT_FOUND               302
#define ERR_VAR_NOT_FOUND               303
#define ERR_BLOCK_EXISTS                304
#define ERR_KEY_EXISTS                  305
#define ERR_VAR_EXISTS                  306

inline void errormsg(int err_num=-1, 
                    int line_number=-1, const 
                    std::string& var_name=""){
    std::string msg = "";
    int err_class = -1;
    switch(err_num){
        case ERR_INVALID_VAR_DECL:
            msg = "Invalid variable declaration: '" + var_name +"' at line " + std::to_string(line_number);
            err_class = ERR_CLASS_FILEHANDLER;
            break;
        case ERR_INVALID_BLOCK_DECL:
            msg = "Invalid block declaration at line " + std::to_string(line_number);
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
        default:
            msg = "Unknown error at line " + std::to_string(line_number);
            break;
    }
    std::cerr << "Error<" << err_class << "?" << err_num << ">: " << msg << std::endl;
}

#endif // FILER_H