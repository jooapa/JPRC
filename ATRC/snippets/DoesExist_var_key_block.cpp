#include "../include/ATRC.hpp"
#include "../include/filer.h"

bool _W_DoesExistBlock_(PATRC_FD filedata, const std::string& block){
    for(Block blk : *filedata->Blocks){
        if(blk.Name == block) return true;
    }
    return false;
}
bool _W_DoesExistVariable_(PATRC_FD filedata, const std::string& varname){
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
bool _W_DoesExistKey_(PATRC_FD filedata, const std::string& block, const std::string& key){
    for(Block blk : *filedata->Blocks){
        if(blk.Name == block){
            for(Key k : blk.Keys){
                if(k.Name == key) return true;
            }
        }
    }
    return false;
}

 bool DoesExistVariable(std::shared_ptr<ATRC_FD> filedata, const std::string& varname){
    return _W_DoesExistVariable_(filedata.get(), varname);
}

 bool DoesExistKey(std::shared_ptr<ATRC_FD> filedata, const std::string& block, const std::string& key){
    return _W_DoesExistKey_(filedata.get(), block, key);
}

 bool DoesExistBlock(std::shared_ptr<ATRC_FD> filedata, const std::string& block){
    return _W_DoesExistBlock_(filedata.get(), block);
}
