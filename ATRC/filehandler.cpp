#include "./include/ATRC.hpp"
#include "./include/filer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <utility>

#define checkblock_success  1
#define checkblock_failure  0
#define checkblock_fatal    2
/// #define checkblock_success  1 
/// #define checkblock_failure  0 
/// #define checkblock_fatal    2 
// when fatal, it will show error
int checkBlock(std::string& _curr_block, const std::string& line, int line_number = -1, std::string filename = "no_filename") {
    if(line[0] != '[') return checkblock_failure;
    int _end_pos = line.find(']');
    if(_end_pos == std::string::npos) {
        errormsg(ERR_INVALID_BLOCK_DECL, line_number, "", filename);
        return checkblock_fatal;
    }
    _curr_block = line.substr(1, _end_pos - 1);
    return checkblock_success;
}

std::string ParseLineSTRINGtoATRC(const std::string &line){
    /*+++
    ! -> \!
    & -> \&
    % -> \%
    %var% -> %var%
    %*[int] -> %*[int]%
    ---*/

    std::string res = "";
    std::string buffer = "";
    bool in_var = false;
    for(const char &c : line){
        if(in_var){
            buffer += c;
            if(c == '%'){
                in_var = false;
                res += buffer;
                buffer = "";
            }
            continue;
        } else {
            if(c == '%'){
                buffer += '%';
                in_var = true;
                continue;
            }
            else if(c == '!'){
                res += "\\!";
                continue;
            }
            else if(c == '&'){
                res += "\\&";
                continue;
            }
        }
        res += c;
    }
    if(buffer[0] == '%'){
        res += '\\' + buffer;
    }
    return res;
}

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

/**
 * reserve characters (needs \ before them):
 * %, !, &
 * reserved sequences:
 * %*%
 * %*<int>%
 * 
 * function trims line input 
 * **line** will be replaced with the value
*/
void ParseLineValueATRCtoSTRING(std::string& line, const int &line_number, const std::unique_ptr<std::vector<Variable>> &variables, const std::string filename) {
    trim(line);
    bool _last_is_re_dash = false;
    bool _looking_for_var = false;
    std::string _value = "";
    std::string _var_name = "";
    for (const char &c : line) {
        if(c == '\\' && _last_is_re_dash) {
            _value += '\\';
            _last_is_re_dash = false;
            continue;
        }
        if (c == '\\') {
            _last_is_re_dash = true;
            continue;
        }   
        
        if(!_last_is_re_dash && c == '!') { // Comment
            // TODO FIX BUG:
            // This is a %*%, and %*% !
            // ->
            // 'This is a %*%, and %*% '
            //
            // This is a %*%, and %*%& !
            // ->
            // 'This is a %*%, and %*%  '
            break; 
        }
        if(!_last_is_re_dash && c == '&') { // Whitespace
            _value += ' ';
            continue;
        }

        if(!_last_is_re_dash && c == '%' || _looking_for_var) { // Variable
            if(!_looking_for_var && c == '%') {
                _looking_for_var = true;
                continue;
            }
            if(_looking_for_var && c == '%') {
                if(variables == nullptr) {
                    errormsg(ERR_NO_VAR_VECTOR, line_number, "", filename);
                    return;
                }
                if(_var_name.empty()) {
                    errormsg(ERR_INVALID_VAR_DECL, line_number, "", filename);
                    return;
                }
                if(_var_name[0] == '*') {
                    _value += "%"+_var_name+"%";
                    _var_name = "";
                    _looking_for_var = false;
                    continue;
                }
                for (Variable var : *variables) {
                    if(var.Name == _var_name) {
                        _value += var.Value;
                        break;
                    }
                }
                _var_name = "";
                _looking_for_var = false;
                continue;
            }
            _var_name += c;
            continue;
        }
        _value += c;
        _last_is_re_dash = false;
    }

    line = _value;
}

std::pair<std::unique_ptr<std::vector<Variable>>, std::unique_ptr<std::vector<Block>>> 
ParseFile(
    const std::string& filename, 
    const std::string& encoding,
    const std::string& extension
) {
    std::unique_ptr<std::vector<Variable>> variables = std::make_unique<std::vector<Variable>>();
    std::unique_ptr<std::vector<Block>>  blocks = std::make_unique<std::vector<Block>>();
    std::vector<Key> keys;

    int extension_int = filename.find_last_of('.');
    if(extension_int == std::string::npos){
        errormsg(ERR_INVALID_FILE, -1, "Invalid file extension, " + filename + " | " + extension, filename);
        return std::make_pair(std::move(variables), std::move(blocks));
    }
    if(filename.substr(extension_int, filename.length() - filename.find_last_of('.')) != extension){
        errormsg(ERR_INVALID_FILE, -1, "Wrong extension, " + filename + " | " + extension, filename);
        return std::make_pair(std::move(variables), std::move(blocks));
    }
    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        errormsg(ERR_INVALID_FILE, -1, "Failed opening: " + filename, filename);
        file.close();
        return std::make_pair(std::move(variables), std::move(blocks));
    }

    // Read through the file line by line
    std::string line;
    int line_number = 0;
    bool _is_re_dash = false;
    std::string _curr_block = "";
    while (std::getline(file, line)) {
        line_number++;
        // Process line
        std::string _line_trim = line;
        ltrim(_line_trim);

        // Skip empty lines and comments
        if (_line_trim == "") continue;
        if (_line_trim[0] == '!') continue;
        

        // Check if line is a variable
        if(_line_trim[0] == '%' || _line_trim.substr(0, 2) == "<%"){
            bool _is_public = false;
            if(_line_trim[0] == '%') _is_public = true;
            // parse name
            int _equ_pos = _line_trim.find('=');
            if(_equ_pos == std::string::npos){
                errormsg(ERR_INVALID_VAR_DECL, line_number, "", filename);
                continue;
            }

            Variable _variable;
            // from %var% = value
            // to %var%
            // to var
            std::string _name = _line_trim.substr(0, _equ_pos);
            trim(_name);
            // if(_is_public) {
            //     _variable.IsPublic = true;
            //     _variable.Name = _name.substr(1, _name.length() - 2);
            // } else {
            //     _variable.IsPublic = false;
            //     _variable.Name = _name.substr(2, _name.length() - 3);
            // }
            _variable.IsPublic = _is_public;
            // true = 1, false = 0
            _variable.Name = _name.substr(2-_is_public, _name.length() - (3 -_is_public));

            if (_variable.Name.empty() || _variable.Name == "*") {
                errormsg(ERR_INVALID_VAR_DECL, line_number, _variable.Name, filename);
                continue;
            }
            if(VariableContainsVariable(variables, _variable)) {
                errormsg(ERR_REREFERENCED_VAR, line_number, _variable.Name, filename);
                continue;
            }
            std::string _value = _line_trim.substr(_equ_pos + 1);
            ParseLineValueATRCtoSTRING(_value, line_number, variables, filename);
            _variable.Value = _value;
            variables->push_back(_variable);
            continue;
        }
        // Check if line is a block
        int check = checkBlock(_curr_block, _line_trim, line_number, filename);
        if(check == checkblock_fatal){
            file.close();
            return std::make_pair(std::move(variables), std::move(blocks));
        }
        if (check == checkblock_success) {
            // Block
            Block block;
            block.Name = _curr_block;
            if(BlockContainsBlock(blocks,block)) {
                errormsg(ERR_REREFERENCED_BLOCK, line_number, block.Name, filename);
                continue;
            }
            blocks->push_back(block);
            continue;
        }
        std::string _key_name = "";
        std::string _key_value = "";
        int _equ_pos = _line_trim.find('=');
        if(_equ_pos == std::string::npos) {
            errormsg(ERR_INVALID_KEY_DECL, line_number, "", filename);
            continue;
        }
        _key_name = _line_trim.substr(0, _equ_pos);
        trim(_key_name);
        _key_value = _line_trim.substr(_equ_pos + 1);
        ParseLineValueATRCtoSTRING(_key_value, line_number, variables, filename);
        Key _key;
        _key.Name = _key_name;
        _key.Value = _key_value;
        if(BlockContainsKey(blocks->back().Keys,_key)) {
            errormsg(ERR_REREFERENCED_KEY, line_number, _key.Name, filename);
            continue;
        }
        blocks->back().Keys.push_back(_key);
    }

    file.close();

    // Will throw error if no blocks or variables are found
    if(variables->empty()){
        Variable _var;
        _var.Name = "var-" + gen_random(10);
        _var.Value = "val-" + gen_random(10);
        variables->push_back(_var);
    } 
    if(blocks->empty()) {
        // append a random block with randomized name
        Block _block;
        _block.Name = "block-" + gen_random(10);
        blocks->push_back(_block);
    }
    return std::make_pair(std::move(variables), std::move(blocks));
}


std::shared_ptr<ATRC_FD> Read(const std::string& filename, const std::string& encoding, const std::string& allowed_extension)
    {
    std::shared_ptr<ATRC_FD> filedata(new ATRC_FD);
    filedata->Filename = filename;
    filedata->Encoding = encoding;
    if(allowed_extension == ""){
        filedata->Extension = ".atrc";
    } else{
        if(allowed_extension[0] == '.'){
            filedata->Extension = allowed_extension;
        } else {
            filedata->Extension = "." + allowed_extension;
        }
    }

    // Parse contents
    auto parsedData = ParseFile(filename, encoding, filedata->Extension);
    // Check for successful parsing
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
    } else {
        filedata->Variables = std::move(parsedData.first);
        filedata->Blocks = std::move(parsedData.second);
    }

    return filedata;
}


void save_final_data(const std::string &filename, const std::string &final_data){
    std::ofstream ofs(filename, std::ofstream::trunc);
    ofs << final_data;
    ofs.close();
}

/*+++
std::ifstream file(filedata->Filename, std::ios::app);
if (!file.is_open()) {
    errormsg(ERR_INVALID_FILE, 
        -1, 
        "Failed opening for saving: " + filedata->Filename, 
        filedata->Filename
        );
    file.close();
    return;
}
while(std::getline(file, line)){
    //HERE
    final_data += line + "\n";
}
file.close();
---*/

/// @brief Save filedata to file
/// @param filedata filedata
/// @param action set empty to or -1 to do heavysave, otherwise set macro
/// @param xtra_info set -2 if not used, send extra info, such as index
/// @param xtra_info2 set "" if not used, send extra info, such as name
void Save
(
    std::shared_ptr<ATRC_FD> filedata, 
    const ATRC_SAVE &action, 
    const int &xtra_info,
    const std::string &xtra_info2,
    const std::string &xtra_info3,
    const std::string &xtra_info4
)
{    
    std::string line = "";
    std::string final_data = "";
    switch (action)
    {
    case ATRC_SAVE::FULL_SAVE:
        
        break;
    case ATRC_SAVE::ADD_BLOCK: {
        std::ofstream file(filedata->Filename, std::ios::app);
        if (!file.is_open()) {
            errormsg(ERR_INVALID_FILE, 
                -1, 
                "Failed opening for saving: " + filedata->Filename, 
                filedata->Filename
                );
            file.close();
            return;
        }
        file << "\n" << "[" << xtra_info2 + "]";
        file.close();
    }
        break;
    case ATRC_SAVE::REMOVE_BLOCK: {
        std::ifstream file(filedata->Filename, std::ios::out);
        if (!file.is_open()) {
            errormsg(ERR_INVALID_FILE, 
                -1, 
                "Failed opening for saving: " + filedata->Filename, 
                filedata->Filename
                );
            
            file.close();
            return;
        }

        bool block_found_ = false;
        bool block_removed = false;
        std::string curr_block = "";
        while(std::getline(file, line)){
            std::string line_trim = line;
            trim(line_trim);
            if(block_removed){
                final_data += line + "\n";
            } else {
                if(line_trim[0] != '['){
                    if(!block_removed && block_found_){
                        continue;
                    }
                    final_data += line + "\n";
                    continue;
                }
                int check = checkBlock(curr_block, line, -1, filedata->Filename);
                if(check == checkblock_failure){
                    file.close();
                    break;
                }
                if(check = checkblock_success){
                    if(block_found_){
                        block_removed = true;
                    }
                    if(curr_block == xtra_info2){
                        block_found_ = true;
                    } else {
                        final_data += line + "\n";
                    }
                }
                
            }
        }
        file.close();
        save_final_data(filedata->Filename, final_data);
        break;
    }
    case ATRC_SAVE::ADD_KEY:
        {
            std::ifstream file(filedata->Filename);
            if (!file.is_open()) {
                errormsg(ERR_INVALID_FILE, 
                    -1, 
                    "Failed opening for saving: " + filedata->Filename, 
                    filedata->Filename
                );
                return;
            }
            bool block_found = false;
            bool key_added = false;
            std::string curr_block = "";
            while (std::getline(file, line)) {
                std::string line_trim = line;
                trim(line_trim);
                if(key_added) {
                    final_data += line + "\n";
                } else {
                    if(line_trim[0] != '['){
                        final_data += line + "\n";
                        continue;
                    }
                    int check = checkBlock(curr_block, line, -1, filedata->Filename);
                    if(check == checkblock_failure){
                        file.close();
                        break;
                    }
                    if(check = checkblock_success){
                        if(curr_block == xtra_info4){
                            block_found = true;
                            final_data += line + "\n";
                            final_data += xtra_info2 + "=" + xtra_info3 + "\n";
                            key_added = true;
                            continue;
                        } else {
                            final_data += line + "\n";
                            continue;
                        }
                    }
                }
            }

            file.close();
            save_final_data(filedata->Filename, final_data);
            break;
        }
        break;
    case ATRC_SAVE::REMOVE_KEY: {
            std::ifstream file(filedata->Filename);
            if (!file.is_open()) {
                errormsg(ERR_INVALID_FILE, 
                    -1, 
                    "Failed opening for saving: " + filedata->Filename, 
                    filedata->Filename
                );
                return;
            }
            bool block_found = false;
            bool key_removed = false;
            std::string curr_block = "";
            while (std::getline(file, line)) {
                std::string line_trim = line;
                trim(line_trim);
                if(key_removed) {
                    final_data += line + "\n";
                } else {
                    if(block_found){
                        int equ_pos = line_trim.find_first_of('=');
                        std::string key_name = line_trim.substr(0, equ_pos);
                        trim(key_name);
                        if(key_name == xtra_info2){
                            key_removed = true;
                            continue;
                        } else{
                            final_data += line + "\n";
                        }
                        continue;
                    }
                    if(line_trim[0] != '['){
                        final_data += line + "\n";
                        continue;
                    }
                    int check = checkBlock(curr_block, line, -1, filedata->Filename);
                    if(check == checkblock_failure){
                        file.close();
                        break;
                    }
                    if(check = checkblock_success){
                        if(curr_block == xtra_info4){
                            block_found = true;
                            final_data += line + "\n";
                            continue;
                        } else {
                            final_data += line + "\n";
                            continue;
                        }
                    }
                }
            }
            file.close();
            save_final_data(filedata->Filename, final_data);
        }
        break;
    case ATRC_SAVE::MODIFY_KEY: {
            std::ifstream file(filedata->Filename);
            if (!file.is_open()) {
                errormsg(ERR_INVALID_FILE, 
                    -1, 
                    "Failed opening for saving: " + filedata->Filename, 
                    filedata->Filename
                );
                return;
            }
            bool block_found = false;
            bool key_modified = false;
            std::string curr_block = "";
            while (std::getline(file, line)) {
                std::string line_trim = line;
                trim(line_trim);
                if(key_modified) {
                    final_data += line + "\n";
                } else {
                    if(block_found){
                        int equ_pos = line_trim.find_first_of('=');
                        std::string key_name = line_trim.substr(0, equ_pos);
                        trim(key_name);
                        if(key_name == xtra_info2){
                            final_data += xtra_info2+"="+xtra_info3+"\n";
                            continue;
                        } else{
                            final_data += line + "\n";
                        }
                        continue;
                    }
                    if(line_trim[0] != '['){
                        final_data += line + "\n";
                        continue;
                    }
                    int check = checkBlock(curr_block, line, -1, filedata->Filename);
                    if(check == checkblock_failure){
                        file.close();
                        break;
                    }
                    if(check = checkblock_success){
                        if(curr_block == xtra_info4){
                            block_found = true;
                            final_data += line + "\n";
                            continue;
                        } else {
                            final_data += line + "\n";
                            continue;
                        }
                    }
                }
            }
            file.close();
            save_final_data(filedata->Filename, final_data);
        }
        break;
    case ATRC_SAVE::ADD_VAR:{
        std::ifstream file(filedata->Filename, std::ios::app);
        if (!file.is_open()) {
            errormsg(ERR_INVALID_FILE, 
                -1, 
                "Failed opening for saving: " + filedata->Filename, 
                filedata->Filename
                );
            file.close();
            return;
        }
        final_data += xtra_info2 + "\n"; //xtra_info2 contains the whole %var%=val
        while(std::getline(file, line)){
            final_data += line + "\n";
        }
        file.close();
        save_final_data(filedata->Filename, final_data);
        break;
    }
    case ATRC_SAVE::REMOVE_VAR: {
        int var_line_num = 0;
        std::ifstream file(filedata->Filename, std::ios::app);
        if (!file.is_open()) {
            errormsg(ERR_INVALID_FILE, 
                -1, 
                "Failed opening for saving: " + filedata->Filename, 
                filedata->Filename
                );
            file.close();
            return;
        }
        while(std::getline(file, line)){
            var_line_num++;
            std::string trim_line = line;
            trim(trim_line);
            if(trim_line[0] == '%'){
                int equ_pos = trim_line.find_first_of('=');
                std::string variable_name = trim_line.substr(0, equ_pos-1);
                trim(variable_name);
                ParseLineValueATRCtoSTRING(line, var_line_num, filedata->Variables, filedata->Filename);
                if(variable_name == "%" + xtra_info2 + "%"){
                    continue;
                }
            }
            final_data += line + "\n";
        }
        file.close();
        save_final_data(filedata->Filename, final_data);
        break;
    }
    case ATRC_SAVE::MODIFY_VAR: {
        std::ifstream file(filedata->Filename, std::ios::app);
        if (!file.is_open()) {
            errormsg(ERR_INVALID_FILE, 
                -1, 
                "Failed opening for saving: " + filedata->Filename, 
                filedata->Filename
                );
            file.close();
            return;
        }
        int var_line_num = 0;
        while(std::getline(file, line)){
            var_line_num++;
            std::string trim_line = line;
            trim(trim_line);
            if(trim_line[0] == '%'){
                int equ_pos = trim_line.find_first_of('=');
                std::string variable_name = trim_line.substr(0, equ_pos-1);
                trim(variable_name);
                ParseLineValueATRCtoSTRING(line, var_line_num, filedata->Variables, filedata->Filename);
                if(variable_name == "%" + xtra_info2 + "%"){
                    final_data += filedata->Variables->at(xtra_info).Name +"="+ filedata->Variables->at(xtra_info).Value + "\n";
                    continue;
                }
            }
            final_data += line + "\n";
        }
        file.close();
        save_final_data(filedata->Filename, final_data);
        break;
    }
    }
}