#include "./include/ATRC.h"
#include "./include/filer.h"
#include <iostream>
#include <fstream>

ATRCFiledata::ATRCFiledata() {
    // Constructor implementation
}

ATRCFiledata::~ATRCFiledata() {
    delete Variables;
    Variables = nullptr;
    delete Blocks;
    Blocks = nullptr;
}
/**
 * reserve characters (needs \ before them):
 * %, !, &
 * reserved sequences:
 * %*%
*/
void ParseLineValueATRCtoSTRING(std::string& line, int line_number) {
    trim(line);
    bool _last_is_re_dash = false;
    for (char c : line) {
        if (c == '\\') {
            _last_is_re_dash = true;
            continue;
        }
    }
}

std::pair<std::vector<Variable>*, std::vector<Block>*> ParseFile(const std::string& filename, const std::string& encoding) {
    std::vector<Variable>* variables = new std::vector<Variable>();
    std::vector<Block>* blocks = new std::vector<Block>();

    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return std::make_pair(nullptr, nullptr);
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
        
        // std::cout << "Line: " << _line_trim << std::endl;


        // Check if line is a variable
        if(_line_trim[0] == '%' || _line_trim.substr(0, 2) == "<%"){
            bool _is_public = false;
            if(_line_trim[0] == '%') _is_public = true;
            // parse name
            int _equ_pos = _line_trim.find('=');
            if(_equ_pos == std::string::npos){
                errormsg(ERR_INVALID_VAR_DECL, line_number);
                continue;
            }

            Variable _variable;
            // from %var% = value
            // to %var%
            std::string _name = _line_trim.substr(0, _equ_pos);
            trim(_name);
            if(_is_public) {
                _variable.IsPublic = true;
                _variable.Name = _name.substr(1, _name.length() - 2);
            } else {
                _variable.IsPublic = false;
                _variable.Name = _name.substr(2, _name.length() - 3);
            }
            if (_variable.Name.empty() || _variable.Name == "*") {
                errormsg(ERR_INVALID_VAR_DECL, line_number, _variable.Name);
                continue;
            }
            std::string _value = _line_trim.substr(_equ_pos + 1);
            ParseLineValueATRCtoSTRING(_value, line_number);
            std::cout << "'" << _variable.Name << "' = '" << _value << "'\n";
            _variable.Value = _value;
            variables->push_back(_variable);
            continue;
        }
        // Check if line is a block
        if (_line_trim[0] == '[') {
            // Block
            Block block;
            block.Name = _line_trim.substr(1, _line_trim.size() - 2);
            blocks->push_back(block);
            _curr_block = block.Name;
            continue;
        }
        
        // Check if line is a key
    }

    std::cout << "File parsed." << std::endl;
    file.close();
    return std::make_pair(variables, blocks);
}

extern "C" ATRCFiledata* Read(const std::string& filename, const std::string& encoding = "utf-8") {
    ATRCFiledata* filedata = new ATRCFiledata;
    filedata->Filename = filename;
    filedata->Encoding = encoding;
    // Parse contents
    std::tie(filedata->Variables, filedata->Blocks) = ParseFile(filename, encoding);

    if(!filedata->Variables || !filedata->Blocks) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
        delete filedata;
        filedata = nullptr;
    }

    return filedata;
}