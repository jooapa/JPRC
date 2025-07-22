#include "./include/ATRC.h"
#include "./include/filer.h"
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

typedef struct _REUSABLE {
    uint64_t line_number;
    std::string filename;
} REUSABLE, * P_REUSABLE;

void _W_ParseLineValueATRCtoSTRING(
    std::string& line, 
    REUSABLE &reus, 
    std::vector<atrc::Variable> &variables
){

}
void ParseLineValueATRCtoSTRING(std::string &value, REUSABLE &reus, std::vector<atrc::Variable> &vars){
    _W_ParseLineValueATRCtoSTRING(value, reus, vars);
}

bool check_and_add_block(std::string &curr_block, std::vector<atrc::Block> &blocks, REUSABLE &reus){
    for(auto &block : blocks) {
        if(block.Name == curr_block) {
            atrc::errormsg(ERR_REREFERENCED_BLOCK, reus.line_number, block.Name, reus.filename);
            return false;
        }
    }
    return true;
}
bool check_and_add_key(std::string &line_trim, std::vector<atrc::Block> &blocks, REUSABLE &reus, std::vector<atrc::Variable> &vars){
    for(auto &block : blocks) {
        if(block.Name == line_trim) {
            atrc::errormsg(ERR_REREFERENCED_KEY, reus.line_number, block.Name, reus.filename);
            return false;
        }
    }
    std::string _key_name = "";
    std::string _key_value = "";
    size_t _equ_pos = line_trim.find('=');
    if(_equ_pos == std::string::npos) {
        atrc::errormsg(ERR_INVALID_KEY_DECL, reus.line_number, "", reus.filename);
        return false;
    }
    _key_name = line_trim.substr(0, _equ_pos);
    atrc::trim(_key_name);
    _key_value = line_trim.substr(_equ_pos + 1);
    ParseLineValueATRCtoSTRING(_key_value, reus, vars);
    atrc::Key _key;
    _key.Name = _key_name;
    _key.Value = _key_value;
    blocks[blocks.size() - 1].Keys.push_back(_key);
    return true;
}

bool check_and_add_variable(std::string &line, std::vector<atrc::Variable> &variables, REUSABLE &reus){
    atrc::Variable var;
    var.IsPublic = false;
    if(line[0] == '%') var.IsPublic = true;
    
    size_t equ = line.find('=');
    if(equ == std::string::npos) {
        atrc::errormsg(ERR_INVALID_VAR_DECL, reus.line_number, "", reus.filename);
        return false;
    }

    var.Name = line.substr(2 - (size_t)var.IsPublic, equ - (3 - (size_t)var.IsPublic));
    if(var.Name.empty() || var.Name == "*") {
        atrc::errormsg(ERR_INVALID_VAR_DECL, reus.line_number, var.Name, reus.filename);
        return false;
    }
    if(atrc::VariableContainsVariable(variables, var)) {
        atrc::errormsg(ERR_REREFERENCED_VAR, reus.line_number, var.Name, reus.filename);
        return false;
    }
    var.Value = line.substr(equ + 1);
    ParseLineValueATRCtoSTRING(var.Value, reus, variables);
    variables.push_back(var);
    return true;
}

uint32_t check_for_block_declaration(std::string &_curr_block, const std::string &line, REUSABLE &reus) {
    if(line[0] != '[') return checkblock_failure;
    size_t end_pos = line.find(']');
    if(end_pos == std::string::npos) {
        atrc::errormsg(ERR_INVALID_BLOCK_DECL, reus.line_number, "", reus.filename);
        return checkblock_fatal;
    }
    _curr_block = line.substr(1, end_pos - 1);
    return checkblock_success;
}

bool atrc::ParseFile
(
    const std::string &_filename, 
    const std::string &encoding, 
    const std::string &extension,
    std::vector<atrc::Variable> &variables,
    std::vector<atrc::Block> &blocks
) {
    blocks.clear();
    variables.clear();
    std::vector<atrc::Key> keys;
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

    REUSABLE reusable;
    reusable.line_number = 0;
    reusable.filename = filename;

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
        if(reusable.line_number++ == 0){
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
            check_and_add_variable(_line_trim, variables, reusable);
            continue;
        }
        // check for block declaration
        uint32_t check = check_for_block_declaration(_curr_block, _line_trim, reusable);
        if(check == checkblock_fatal){
            file.close();
            return false;
        }
        if (check == checkblock_success) {
            check_and_add_block(_curr_block, blocks, reusable);
            continue;
        }
        // check and add key to block
        check_and_add_key(_line_trim, blocks, reusable, variables);
    }
    file.close();
    return true;
}