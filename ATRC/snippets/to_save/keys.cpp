#include "../../include/ATRC.hpp"
#include "../../include/filer.h"
#include <string>
/*
    ! TEST EVERYTHING
*/

void _W_AddKey_(PATRC_FD filedata, const std::string& block, const std::string& key, const std::string& value){
    if(!_W_DoesExistBlock_(filedata, block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, filedata->Filename);
        return;
    }
    if(_W_DoesExistKey_(filedata, block, key)){
        errormsg(ERR_KEY_EXISTS, -1, key, filedata->Filename);
        return;
    }

    Key _key;
    _key.Name = key;
    _key.Value = value;
    for(Block &_block : *filedata->Blocks){
        if(_block.Name == block){
            _block.Keys.push_back(_key);
            break;
        }
    }
    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::ADD_KEY, -1, key, value, block);
    }
}
void _W_RemoveKey_(PATRC_FD filedata, const std::string& block, const std::string& key){
    if(!_W_DoesExistBlock_(filedata, block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, filedata->Filename);
        return;
    }
    if(!_W_DoesExistKey_(filedata, block, key)){
        errormsg(ERR_KEY_NOT_FOUND, -1, key, filedata->Filename);
        return;
    }
    int i = 0;
    for (Block &_block : *filedata->Blocks) {
        if (_block.Name == block) {
            auto it = _block.Keys.begin();
            while (it != _block.Keys.end()) {
                if (it->Name == key) {
                    it = _block.Keys.erase(it);
                } else {
                    ++it;
                    i++;
                }
            }
            break;
        }
    }


    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::REMOVE_KEY, i, key, "", block);
    }
}
void _W_ModifyKey_(PATRC_FD filedata, const std::string& block, const std::string& key, const std::string& value){
    if(!_W_DoesExistBlock_(filedata, block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, filedata->Filename);
        return;
    }
    if(!_W_DoesExistKey_(filedata, block, key)){
        errormsg(ERR_KEY_NOT_FOUND, -1, key, filedata->Filename);
        return;
    }

    int i = 0;
    for(Block &_blk : *filedata->Blocks){
        if(_blk.Name == block){
            for(Key &_key : _blk.Keys){
                if(_key.Name == key){
                    _key.Value = value;
                    break;
                }
                i++;
            }
            break;
        }
    }

    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::MODIFY_KEY, i, key, value, block);
    }
}


void AddKey
(
std::shared_ptr<ATRC_FD> filedata, 
const std::string &block, 
const std::string &key, 
const std::string &value
){
    return _W_AddKey_(filedata.get(), block, key, value);
}

void RemoveKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string &block, 
    const std::string &key
){
    return _W_RemoveKey_(filedata.get(), block, key);
}

void ModifyKey(
std::shared_ptr<ATRC_FD> filedata,
const std::string &block,
const std::string &key,
const std::string &value
){
    return _W_ModifyKey_(filedata.get(), block, key, value);
}