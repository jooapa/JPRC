#ifndef FILER_H
#define FILER_H
#include <string>
#include <iostream>
#include <vector>
#include "ATRC.h"


bool BlockContainsKey(std::vector<Key>* keys, Key* key);
bool BlockContainsBlock(std::vector<Block>* blocks, Block* block);
bool VariableContainsVariable(std::vector<Variable>* variables, Variable* variable);

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

#define ERR_CLASS_FILEHANDLER 100
#define ERR_INVALID_VAR_DECL 1
#define ERR_INVALID_BLOCK_DECL 2
#define ERR_INVALID_KEY_DECL 3
#define ERR_NO_VAR_VECTOR 4
#define ERR_REREFERENCED_VAR 5
#define ERR_REREFERENCED_BLOCK 6
#define ERR_REREFERENCED_KEY 7    

inline void errormsg(int err_num=-1, int line_number=-1, const std::string& var_name=""){
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
        default:
            msg = "Unknown error at line " + std::to_string(line_number);
            break;
    }
    std::cerr << "Error<" << err_class << "-" << err_num << ">: " << msg << std::endl;
}

#endif // FILER_H