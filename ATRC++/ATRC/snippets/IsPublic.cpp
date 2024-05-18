#include "../include/ATRC.h"

extern "C" bool IsPublic(ATRCFiledata* filedata, const std::string& varname){
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            return var.IsPublic;
        } 
    }
    return false;
}