#include "../include/ATRC.h"
#include "../include/filer.h"

extern "C" void ReadVariable(ATRCFiledata *filedata, const std::string& varname, std::string& contents) {
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            if(var.IsPublic) {
                contents = var.Value;
            } else {
                errormsg(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname);
                contents = "";
            }
            return;
        }
    }
    contents = "";
}