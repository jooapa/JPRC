#include "./include/ATRC.h"
#include <iostream>
#include <fstream>
std::pair<std::vector<Variable>*, std::vector<Block>*> ParseFile(const std::string& filename, const std::string& encoding);
std::string ParseLineValue(const std::string& line, int line_number);

ATRCFiledata::ATRCFiledata() {
    // Constructor implementation
}

ATRCFiledata::~ATRCFiledata() {
    delete Variables;
    delete Blocks;
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
        return nullptr;
    }

    return filedata;
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
    while (std::getline(file, line)) {
        std::string value = ParseLineValue(line, line_number);
        std::cout << "Line " << line_number << ": " << value << std::endl;
        line_number++;
    }

    std::cout << "File parsed." << std::endl;
    file.close();
    return std::make_pair(variables, blocks);
}


std::string ParseLineValue(const std::string& line, int line_number) {
    // Parse line and return value
    int equals_pos = line.find('=');

    std::string lineCopy = line.substr(equals_pos + 1);
    trim(lineCopy);
    for (char c : lineCopy) {
    }
    return lineCopy;
}