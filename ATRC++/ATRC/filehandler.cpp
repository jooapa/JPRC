#include "./include/ATRC.h"
#include "./include/filer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <utility>

bool checkBlock(std::string& _curr_block, const std::string& line, int line_number = -1, std::string filename = "no_filename") {
    if(line[0] != '[') return false;
    int _end_pos = line.find(']');
    if(_end_pos == std::string::npos) {
        errormsg(ERR_INVALID_BLOCK_DECL, line_number, "", filename);
        throw std::invalid_argument("Invalid block declaration");
    }
    _curr_block = line.substr(1, _end_pos - 1);
    return true;
}

void ParseLineSTRINGtoATRC(std::string &line);

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
*/
void ParseLineValueATRCtoSTRING(std::string& line, int line_number, std::unique_ptr<std::vector<Variable>> &variables, std::string filename) {
    trim(line);
    bool _last_is_re_dash = false;
    bool _looking_for_var = false;
    std::string _value = "";
    std::string _var_name = "";
    for (char c : line) {
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
    std::string& filename, 
    const std::string& encoding
) {
    std::unique_ptr<std::vector<Variable>> variables = std::make_unique<std::vector<Variable>>();
    std::unique_ptr<std::vector<Block>>  blocks = std::make_unique<std::vector<Block>>();
    std::vector<Key> keys;
    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
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
        if (_line_trim.empty()) continue;
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
            if(VariableContainsVariable(variables, &_variable)) {
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
        if (checkBlock(_curr_block, _line_trim, line_number, filename)) {
            // Block
            Block block;
            block.Name = _curr_block;
            if(BlockContainsBlock(blocks, &block)) {
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
        if(BlockContainsKey(blocks->back().Keys, &_key)) {
            errormsg(ERR_REREFERENCED_KEY, line_number, _key.Name, filename);
            continue;
        }
        blocks->back().Keys.push_back(_key);
    }

    file.close();

    // Will throw error if no blocks or variables are found
    if(variables->empty()){
        Variable _var;
        _var.Name = "var" + gen_random(5);
        _var.Value = "val" + gen_random(5);
        variables->push_back(_var);
    } 
    if(blocks->empty()) {
        // append a random block with randomized name
        Block _block;
        _block.Name = "block" + gen_random(5);
        blocks->push_back(_block);
    }
    return std::make_pair(std::move(variables), std::move(blocks));
}

std::unique_ptr<ATRCFiledata> Read(std::string& filename, const std::string& encoding = "utf-8", const std::string& allowed_extension) {
    auto filedata = std::make_unique<ATRCFiledata>();
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
    auto parsedData = ParseFile(filename, encoding);
    // Check for successful parsing
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
    } else {
        filedata->Variables = std::move(parsedData.first);
        filedata->Blocks = std::move(parsedData.second);
    }

    return filedata;
}

/// @brief Save filedata to file
/// @param filedata filedata
/// @param action set empty to or -1 to do heavysave, otherwise set macro
/// @param xtra_info set -2 if not used, send extra info, such as index
/// @param xtra_info2 set "" if not used, send extra info, such as name
void Save(ATRCFiledata *filedata, int action, int xtra_info, std::string xtra_info2) {
    switch (action)
    {
    case -1: break; //hard save
    case AUTOSAVE_ADD_BLOCK: break;
    case AUTOSAVE_REMOVE_BLOCK: break;
    case AUTOSAVE_ADD_KEY: break;
    case AUTOSAVE_REMOVE_KEY: break;
    case AUTOSAVE_MODIFY_KEY: break;
    case AUTOSAVE_ADD_VAR: break;
    case AUTOSAVE_REMOVE_VAR: break;
    case AUTOSAVE_MODIFY_VAR: break;
    default:break;
    }
    // file.close();
}