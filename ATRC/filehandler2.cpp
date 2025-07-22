#include "ATRC.h"
#include "filer.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <filesystem>
#include <stack>
#include <vector>
#include <unordered_set>
#define checkblock_success  1
#define checkblock_failure  0
#define checkblock_fatal    2


/*+++
Preprocessor handling functions, parsing, constants, etc.
---*/
bool evaluatePlatformTag(const std::string &tag) {
#ifdef __linux__
    return tag == "LINUX";
#elif _WIN32 || _WIN64
    return tag == "WINDOWS";
#elif __APPLE__ || __MACH__
    return tag == "MACOS";
#elif __unix__
    return tag == "UNIX";
#else
    return false;
#endif
}

bool evaluateArchitectureTag(const std::string &tag) {
#if defined(__x86_64__) || defined(_M_X64)
    return tag == "X64";
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    return tag == "X86";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return tag == "ARM64";
#elif defined(__arm__) || defined(_M_ARM)
    return tag == "ARM";
#else
    return false;
#endif
}

bool evaluateLogicalOperator(const std::string &op, std::list<bool> &results, size_t index) {
    if (op == "NOT") {
        if (results.empty()) return false; // Syntax error
        bool value = results.back();
        results.pop_back();
        return !value;
    }

    if (results.size() < 2) return false; // Syntax error

    bool rhs = results.back();
    results.pop_back();
    bool lhs = results.back();
    results.pop_back();

    if (op == "AND") return lhs && rhs;
    if (op == "OR") return lhs || rhs;

    return false; // Unsupported operator
}

enum class PPRes {
    SeeContents,        // Flag is solved in the preprocessor block
    DontSeeContents,    // Flag is not solved in the preprocessor block
    EndIF,              // End of the preprocessor block
    Normal,             // Normal return
    Ignore,             // Ignore n lines
    InvalidFlag,        // Invalid flag
    InvalidFlagContents,// Invalid flag contents

    ThreeParts,         // .FLAG=<var>=<value>
    Undefine,           // .UNDEF <var>
    Message,            // .LOG, .WARNING, .ERROR...
};

/*
    ".IF",      // If               -> .IF <tag> <tag> ...
    ".ELSEIF",  // Else if          -> .ELSEIF <tag> <tag> ...
    ".ELSE",    // Else             -> .ELSE
    ".ENDIF",   // End if           -> .ENDIF
    
    ".DEFINE",  // Define           -> .DEFINE <var>=<value>
    ".UNDEF",   // Undefine         -> .UNDEF <var>

    ".LOG",     // Log to console   -> .LOG <message>           // Logs to stdout, no color
    ".WARNING", // Warning message  -> .WARNING <message>       // Logs to stdout, yellow
    ".ERROR",   // Error message    -> .ERROR <message>         // Logs to stderr, red
    ".ERRORCOUT", // Error message  -> .ERRORCOUT <message>     // Logs to stdout, red
    ".SUCCESS", // Success message  -> .SUCCESS <message>       // Logs to stdout, green
    ".DEBUG",   // Debug message    -> .DEBUG <message>         // Logs to stdout, cyan
    
    ".IGNORE",  // Ignore n lines   -> .IGNORE <n>

    Operating systems
    "LINUX",
    "WINDOWS",
    "MACOS",
    "UNIX",

    Architectures
    "X86",
    "X64",
    "ARM",
    "ARM64",

    Logical operators
    "OR",
    "AND",
    "NOT",
*/
const std::unordered_set<std::string> PREPROCESSOR_FLAGS = {
    ".IF", ".ELSEIF", ".ELSE", ".ENDIF", 
    ".DEFINE", ".UNDEF", 
    ".LOG", ".WARNING", ".ERROR", ".ERRORCOUT", ".SUCCESS", ".DEBUG",
    ".IGNORE"
};
const std::unordered_set<std::string> PREPROCESSOR_TAGS = {
    "LINUX", "WINDOWS", "MACOS", "UNIX", 
    "X86", "X64", "ARM", "ARM64", 
    "AND", "OR", "NOT"
};

// Struct: Preprocessor block metadata
struct PreprocessorBlock {
    std::string flag;           // Preprocessor flag
    std::string flag_contents;  // Contents following the flag
    bool isSolved = false;      // Whether the condition is solved
    bool solvedResult = false;  // Result of the solved condition
    int64_t numeral_data = 0;   // Numeric data (e.g., for .IGNORE)
    std::string string_data;    // String data (e.g., for .LOG)
    std::string var_name;       // Variable name (e.g., for .DEFINE)
    std::string var_value;      // Variable value (e.g., for .DEFINE)
    std::vector<std::string> lines; // Lines within the preprocessor block
};


/*+++
Parses preprocessor line:

#<flag> <contents...>
-> 
{
    flag: <flag>
    flag_contents: <contents>
    isSolved: false|true        // Flag is solved in the preprocessor block
    solvedResult: false|true    // Result of the flag
    lines: [] // lines that are inside the preprocessor block. Will be parsed outside this function
}
---*/
PPRes parsePreprocessorFlag(const std::string &line, PreprocessorBlock &block, const std::string &filename){
    PPRes res = PPRes::Normal;
    return res;
}

/*+++
Parsing functions for ATRC
---*/


void _W_ParseLineValueATRCtoSTRING(std::string& line, const atrc::REUSABLE &reus, const std::vector<atrc::Variable> &variables) {
    atrc::trim(line);
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
        
        if(!_last_is_re_dash && c == '#') { // Comment
            _value = _value.substr(0, _value.length() - 1);
            atrc::trim(_value);
            break; 
        }
        if(!_last_is_re_dash && c == '&') { // Whitespace
            _value += ' ';
            continue;
        }

        if((!_last_is_re_dash && c == '%') || _looking_for_var) { // atrc::Variable
            if(!_looking_for_var && c == '%') {
                _looking_for_var = true;
                continue;
            }
            if(_looking_for_var && c == '%') {
                if(variables.empty()) {
                    atrc::errormsg(ERR_NO_VAR_VECTOR, (int)reus.line_number, "", reus.filename);
                    return;
                }
                if(_var_name.empty()) {
                    atrc::errormsg(ERR_INVALID_VAR_DECL, (int)reus.line_number, "", reus.filename);
                    return;
                }
                if(_var_name[0] == '*') {
                    _value += "%"+_var_name+"%";
                    _var_name = "";
                    _looking_for_var = false;
                    continue;
                }
                for (const atrc::Variable &var : variables) {
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
void atrc::ParseLineValueATRCtoSTRING(std::string& line, const atrc::REUSABLE &reus, const std::vector<atrc::Variable> &variables) {
    _W_ParseLineValueATRCtoSTRING(line, reus, variables);
}

std::string atrc::ParseLineSTRINGtoATRC(const std::string &line) {
    /*+++
    # -> \#
    & -> \&
    % -> \%
    %var% -> %var%
    %*[int] -> %*[int]%
    */
    std::string res = "";
    std::string buffer = "";
    bool in_var = false;
    for (const char &c : line) {
        if(in_var) {
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
            else if(c == '#'){
                res += "\\#";
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

bool check_and_add_block(std::string &curr_block, std::vector<atrc::Block> &blocks, atrc::REUSABLE &reus){
    for(auto &block : blocks) {
        if(block.Name == curr_block) {
            atrc::errormsg(ERR_REREFERENCED_BLOCK, (int)reus.line_number, block.Name, reus.filename);
            return false;
        }
    }
    atrc::Block _block;
    _block.Name = curr_block;
    blocks.push_back(_block);
    if(blocks.size() == -1) {
        atrc::errormsg(ERR_INVALID_BLOCK_DECL, (int)reus.line_number, curr_block, reus.filename);
        return false;
    }
    return true;
}
bool check_and_add_key(std::string &line_trim, std::vector<atrc::Block> &blocks, atrc::REUSABLE &reus, std::vector<atrc::Variable> &vars){
    for(auto &block : blocks) {
        if(block.Name == line_trim) {
            atrc::errormsg(ERR_REREFERENCED_KEY, (int)reus.line_number, block.Name, reus.filename);
            return false;
        }
    }
    std::string _key_name = "";
    std::string _key_value = "";
    size_t _equ_pos = line_trim.find('=');
    if(_equ_pos == std::string::npos) {
        atrc::errormsg(ERR_INVALID_KEY_DECL, (int)reus.line_number, "", reus.filename);
        return false;
    }
    _key_name = line_trim.substr(0, _equ_pos);
    atrc::trim(_key_name);
    _key_value = line_trim.substr(_equ_pos + 1);
    atrc::ParseLineValueATRCtoSTRING(_key_value, reus, vars);
    atrc::Key _key;
    _key.Name = _key_name;
    _key.Value = _key_value;
    size_t last_block_index = blocks.size() - 1;
    try {
        blocks[last_block_index].Keys.push_back(_key);
    } catch (const std::exception &e) {
        std::cerr << "Error adding key: " << e.what() << std::endl;
        atrc::errormsg(ERR_INVALID_KEY_DECL, (int)reus.line_number, _key_name, reus.filename);
        return false;
    }
    return true;
}
bool check_and_add_variable(std::string &line, std::vector<atrc::Variable> &variables, atrc::REUSABLE &reus){
    atrc::Variable var;
    var.IsPublic = false;
    if(line[0] == '%') var.IsPublic = true;
    
    size_t equ = line.find('=');
    if(equ == std::string::npos) {
        atrc::errormsg(ERR_INVALID_VAR_DECL, (int)reus.line_number, "", reus.filename);
        return false;
    }

    var.Name = line.substr(2 - (size_t)var.IsPublic, equ - (3 - (size_t)var.IsPublic));
    if(var.Name.empty() || var.Name == "*") {
        atrc::errormsg(ERR_INVALID_VAR_DECL, (int)reus.line_number, var.Name, reus.filename);
        return false;
    }
    if(atrc::VariableContainsVariable(variables, var)) {
        atrc::errormsg(ERR_REREFERENCED_VAR, (int)reus.line_number, var.Name, reus.filename);
        return false;
    }
    var.Value = line.substr(equ + 1);
    atrc::ParseLineValueATRCtoSTRING(var.Value, reus, variables);
    variables.push_back(var);
    return true;
}
uint32_t check_for_block_declaration(std::string &_curr_block, const std::string &line, atrc::REUSABLE &reus) {
    if(line[0] != '[') return checkblock_failure;
    size_t end_pos = line.find(']');
    if(end_pos == std::string::npos) {
        atrc::errormsg(ERR_INVALID_BLOCK_DECL, (int)reus.line_number, "", reus.filename);
        return checkblock_fatal;
    }
    _curr_block = line.substr(1, end_pos - 1);
    return checkblock_success;
}

bool atrc::ParseFile(const std::string &_filename, const std::string &encoding, const std::string &extension, std::vector<atrc::Variable> &variables,std::vector<atrc::Block> &blocks) {
    blocks.clear();
    variables.clear();
    std::vector< std::pair<std::string, std::string> > definitions;
    if(encoding == "" || extension == ""){} // Ignore for now
    std::string filename = _filename;
    if(!std::filesystem::exists(filename)){
        atrc::errormsg(ERR_INVALID_FILE, -1, "File does not exist: " + filename, filename);
        return false;
    }
    std::ifstream file(filename);
    if (!file.is_open()) {
        atrc::errormsg(ERR_INVALID_FILE, -1, "Failed opening: " + filename, filename);
        file.close();
        return false;
    }

    atrc::REUSABLE REUSABLE;
    REUSABLE.line_number = 0;
    REUSABLE.filename = filename;

    // Read through the file line by line
    std::string line;
    std::string _curr_block = "";

    //std::stack<PreprocessorBlock> _preprocessor_blocks;
    //bool inside_preprocessor_block = false;
    //bool solved_preprocessor_block = false;
    //bool wait_for_new_block = false;
    uint64_t _ignore_lines = 0;
    
    while (std::getline(file, line)) {
        std::string _line_trim = line;
        atrc::trim(_line_trim);
        if(REUSABLE.line_number++ == 0){
            std::string FL_HEADER = "#!ATRC";
            if(_line_trim != FL_HEADER){
                atrc::errormsg(ERR_INVALID_FILE, -1, "Invalid file header: " + filename + " Is: '" + _line_trim+"'", filename);
                file.close();
                return false;
            }
            continue;
        }
        if (_line_trim.empty()) continue;
        if (_line_trim[0] == '#' 
        // || inside_preprocessor_block
        // || solved_preprocessor_block
        // || wait_for_new_block
        ) {
            // check if line is a preprocessor directive
            continue;
        }
        if(_ignore_lines > 0) { //preprocessor ignoring
            _ignore_lines--;
            continue;
        }

        // TODO: Handle preprocessor blocks

        // check for variable declaration
        if(_line_trim[0] == '%' || _line_trim.substr(0, 2) == "<%") {
            check_and_add_variable(_line_trim, variables, REUSABLE);
            continue;
        }
        // check for block declaration
        uint32_t check = check_for_block_declaration(_curr_block, _line_trim, REUSABLE);
        if(check == checkblock_fatal){
            file.close();
            return false;
        }
        if (check == checkblock_success) {
            check_and_add_block(_curr_block, blocks, REUSABLE);
            continue;
        }
        // check and add key to block
        check_and_add_key(_line_trim, blocks, REUSABLE, variables);
    }
    file.close();
    return true;
}

/*+++
File saving functions
---*/
void save_final_data(const std::string &filename, const std::string &final_data){
    std::ofstream ofs(filename, std::ofstream::trunc);
    ofs << final_data;
    ofs.close();
}


void add_to_top_of_file(std::string line_to_add, std::string &final_data, const std::vector<char> &add_after_chars, std::ifstream &file, uint64_t &ln_num) {
    bool added = false;
    std::string line;
    while(std::getline(file, line)){
        std::string line_trim = line;
        atrc::trim(line_trim);
        if(ln_num++ == 0) {
            final_data += line + "\n";
            continue;
        }
        bool skip_line = false;
        for(char c : add_after_chars) {
            if(line_trim[0] == c) {
                final_data += line + "\n";
                skip_line = true;
                break;
            }
        }
        if(skip_line) continue;
        if(!added) {
            final_data += line_to_add + "\n";
            added = true;
        }
        final_data += line + "\n";
    }
    if(!added) {
        final_data += line_to_add + "\n";
        added = true;
    }
}

void atrc::_W_Save_(ATRC_FD *filedata, const atrc::ATRC_SAVE &action, const int &xtra_info, const std::string &xtra_info2,const std::string &xtra_info3,const std::string &xtra_info4){
    std::string line = "";
    std::string final_data = "";
    std::ifstream file(filedata->GetFilename(), std::ios::app);
    atrc::REUSABLE reus;
    reus.line_number = xtra_info;
    reus.filename = filedata->GetFilename();
    if (!file.is_open()) {
        atrc::errormsg(ERR_INVALID_FILE, 
            -1, 
            "Failed opening for saving: " + filedata->GetFilename(), 
            filedata->GetFilename()
            );
        file.close();
        return;
    }
    uint64_t ln_num = 0;
    switch (action) {
    case atrc::ATRC_SAVE::FULL_SAVE: {} break;
    case atrc::ATRC_SAVE::ADD_BLOCK: {
        std::string block = "[" + xtra_info2 + "]\n";
        add_to_top_of_file(block, final_data, {'#', '%', '<'}, file, ln_num);
        file.close(); 
        save_final_data(filedata->GetFilename(), final_data);
    } break;
    case atrc::ATRC_SAVE::REMOVE_BLOCK: {
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
                int check = check_for_block_declaration(curr_block, line, reus);
                if(check == checkblock_failure){
                    file.close();
                    break;
                }
                if(check ==checkblock_success){
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
        save_final_data(filedata->GetFilename(), final_data);
    } break;
    case atrc::ATRC_SAVE::ADD_KEY: {
        bool key_added = false;
        std::string curr_block = "";
        std::string key_name = xtra_info2;
        std::string key_value = xtra_info3;
        std::string block_name = xtra_info4;
        while(std::getline(file, line)){
            std::string line_trim = line;
            trim(line_trim);
            if(key_added) {
                final_data += line + "\n";
            } else {
                if(line_trim[0] != '['){
                    final_data += line + "\n";
                    continue;
                }
                int check = check_for_block_declaration(curr_block, line, reus);
                if(check == checkblock_failure){
                    file.close();
                    break;
                }
                if(check ==checkblock_success){
                    if(curr_block == block_name){
                        final_data += line + "\n";
                        final_data += key_name + "=" + key_value + "\n";
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
        save_final_data(filedata->GetFilename(), final_data);
    } break;
    case atrc::ATRC_SAVE::REMOVE_KEY: {} break;
    case atrc::ATRC_SAVE::MODIFY_KEY: {} break;
    case atrc::ATRC_SAVE::ADD_VAR: {
        bool var_added = false;
        std::string var_line = xtra_info2+"\n";
        add_to_top_of_file(var_line, final_data, {'#'}, file, ln_num);
        file.close(); 
        save_final_data(filedata->GetFilename(), final_data);
    } break;
    case atrc::ATRC_SAVE::REMOVE_VAR: {} break;
    case atrc::ATRC_SAVE::MODIFY_VAR: {} break;
    default:
        atrc::errormsg(ERR_INVALID_SAVE_ACTION, -1, "", filedata->GetFilename());
        return;
    }
    if(file.is_open()) file.close();
}