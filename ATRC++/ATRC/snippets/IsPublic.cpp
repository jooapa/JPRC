#include "../include/ATRC.h"

 bool IsPublic(ATRCFiledata* filedata, const std::string& varname){
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            return var.IsPublic;
        } 
    }
    return false;
}