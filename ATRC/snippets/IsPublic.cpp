#include "../include/ATRC.hpp"
bool _W_IsPublic_(PATRC_FD filedata, const std::string& varname){
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            return var.IsPublic;
        } 
    }
    return false;
}
 bool IsPublic(std::shared_ptr<ATRC_FD> filedata, const std::string& varname){
    return _W_IsPublic_(filedata.get(), varname);
}