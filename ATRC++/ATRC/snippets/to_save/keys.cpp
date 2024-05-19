#include "..\..\include\ATRC.h"
#include "..\..\include\filer.h"
#include <string>

extern "C" void AddKey
                    (
                        ATRCFiledata *filedata, 
                        const std::string &block, 
                        const std::string &key, 
                        const std::string &value
                    ){
    
    // TODO Change function
    if(DoesExistKey(filedata, block, key)){
        errormsg(ERR_KEY_EXISTS, -1, key);
        return;
    }

    Key _key;
    _key.Name = key;
    _key.Value = value;
    for(Block _block : *filedata->Blocks){
        if(_block.Name == block){
            _block.Keys.push_back(_key);
            break;
        }
    }
    if(filedata->AutoSave){
        Save(filedata, AUTOSAVE_ADD_KEY);
    }
}

extern "C" void RemoveKey
                    (
                        ATRCFiledata *filedata, 
                        const std::string &block, 
                        const std::string &key
                    ){
    // TODO Change function
    if(!DoesExistKey(filedata, block, key)){
        errormsg(ERR_KEY_NOT_FOUND, -1, key);
        return;
    }

    

    if(filedata->AutoSave){
        Save(filedata, AUTOSAVE_REMOVE_KEY);
    }
}