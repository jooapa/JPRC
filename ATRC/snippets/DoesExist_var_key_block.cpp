#include "../include/ATRC.hpp"
#include "../include/filer.h"

 bool DoesExistVariable(ATRC_FD* filedata, const std::string& varname){
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            if(var.IsPublic) return true;
            else{
                errormsg(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, filedata->Filename);
                return false;            
            }
        } 
    }
    return false;
}

 bool DoesExistKey(ATRC_FD* filedata, const std::string& block, const std::string& key){
    for(Block blk : *filedata->Blocks){
        if(blk.Name == block){
            for(Key k : blk.Keys){
                if(k.Name == key) return true;
            }
        }
    }
    return false;
}

 bool DoesExistBlock(ATRC_FD* filedata, const std::string& block){
    for(Block blk : *filedata->Blocks){
        if(blk.Name == block) return true;
    }
    return false;
}
