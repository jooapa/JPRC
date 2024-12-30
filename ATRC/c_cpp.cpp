#include <ATRC.h>
#include <iostream>
#include <string>
#include <vector>
#include <filer.h>
/*+++
Wrap around functions from C++ to c
---*/
bool _ATRC_WRAP_READ(C_PATRC_FD self) {
    std::string filename = self->Filename;
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
    auto parsedData = atrc::ParseFile(filename, encoding, extension);
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
        return false;
    } else {
        // TODO
    }
    return true;
}

void _ATRC_WRAP_ERRORMSG(int err_num, int line_number, const char *var_name, const char *filename){
    atrc::errormsg(err_num, line_number, var_name, filename);
}