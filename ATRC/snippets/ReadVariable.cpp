#include "../include/ATRC.hpp"
#include "../include/filer.h"

std::string ReadVariable(ATRC_FD *filedata, const std::string& varname) {
    std::string res = "";
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            if(var.IsPublic) {
                res = var.Value;
            } else {
                errormsg(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, filedata->Filename);
                res = "";
            }
            return res;
        }
    }
    res = "";
    return res;
}