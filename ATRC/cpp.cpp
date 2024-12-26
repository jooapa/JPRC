#include <DEV_INC.hpp>
#include <filer.h>
/*+++
Wrap around functions from C++ to c
---*/
bool _ATRC_WRAP_READ(PATRC_FD self){
    std::string filename = self->Filename;
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
    auto parsedData = ParseFile(filename, encoding, extension);
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
        return false;
    } else {
        C_Variable *vars = (C_Variable*)malloc(sizeof(C_Variable) * parsedData.first->size());
        for(int i = 0; i < parsedData.first->size(); i++){
            vars[i].Name = parsedData.first->at(i).Name.c_str();
            vars[i].Value = parsedData.first->at(i).Value.c_str();
            vars[i].IsPublic = parsedData.first->at(i).IsPublic;
        }
        self->Variables = vars;

        C_Block *blocks = (C_Block*)malloc(sizeof(C_Block) * parsedData.second->size());
        for(int i = 0; i < parsedData.second->size(); i++){
            blocks[i].Name = parsedData.second->at(i).Name.c_str();
            blocks[i].KeyCount = parsedData.second->at(i).Keys.size();
            C_Key *keys = (C_Key*)malloc(sizeof(C_Key) * parsedData.second->at(i).Keys.size());
            for(int j = 0; j < parsedData.second->at(i).Keys.size(); j++){
                keys[j].Name = parsedData.second->at(i).Keys[j].Name.c_str();
                keys[j].Value = parsedData.second->at(i).Keys[j].Value.c_str();
            }
            blocks[i].Keys = keys;
        }
        self->Blocks = blocks;
    }
    return true;
}




std::string ATRCFiledata::ReadVariable(const std::string& varname){
    std::string res = "";
    for(Variable var : *this->Variables){
        if(var.Name == varname){
            if(var.IsPublic) {
                res = var.Value;
            } else {
                errormsg(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, this->Filename);
                res = "";
            }
            return res;
        }
    }
    res = "";
    return res;
}
std::string ATRCFiledata::ReadKey(const std::string& block, const std::string& key){
    std::string res = "";
    for(Block blk : *this->Blocks){
        if(blk.Name == block){
            for(Key k : blk.Keys){
                if(k.Name == key){
                    res = k.Value;
                    return res;
                }
            }
        }
    }
    res = "";
    return res;
}
std::string ATRCFiledata::InsertVar_S(const std::string &line, std::vector<std::string> &args){
    std::string res = line;
    InsertVar(res, args);
    return res;
}
bool ATRCFiledata::DoesExistBlock(const std::string& block){
    for(Block &blk : *this->Blocks){
        if(blk.Name == block) return true;
    }
    return false;
}
bool ATRCFiledata::DoesExistVariable(const std::string& varname){
    for(Variable& var : *this->Variables){
        if(var.Name == varname){
            if(var.IsPublic) return true;
            else{
                errormsg(ERR_UNAUTHORIZED_ACCESS_TO_VAR, -1, varname, this->Filename);
                return false;            
            }
        } 
    }
    return false;
}
bool ATRCFiledata::DoesExistKey(const std::string& block, const std::string& key){
    for(Block &blk : *this->Blocks){
            if(blk.Name == block){
                for(Key &k : blk.Keys){
                    if(k.Name == key) return true;
                }
            }
        }
        return false;
}
bool ATRCFiledata::IsPublic(const std::string& varname){
    for(Variable &var : *this->Variables){
        if(var.Name == varname){
            return var.IsPublic;
        } 
    }
    return false;
}
void ATRCFiledata::InsertVar(std::string &line, std::vector<std::string> &args){
    bool _looking_for_var = false;
    std::string var;
    std::string _result;
    int _arg_counter = 0;

    for (size_t i = 0; i < line.length(); ++i) {
        if (line[i] == '\0') break;
        if (line[i] == '%' || _looking_for_var) {
            var += line[i];
            if (line[i] == '%') {
                if (_looking_for_var) {
                    

                    if (var.substr(0, 2) == "%*") {

                        if(var == "%*%"){
                            _result += args[_arg_counter++];
                        } else {
                            
                            // check %*<digit>%
                            if(var.size() > 3) {
                                // size_t inject_num = std::stoull(var.substr(2, var.size() - 3));
                                size_t inject_num = std::stoi(var.substr(2, var.size() - 3));
                                if(args.size() > inject_num){
                                    _result += args[inject_num];
                                } else {
                                    errormsg(ERR_INSERT_WRONG, -1, var + " inject: " + std::to_string(inject_num), this->Filename);
                                }
                            } else {
                                errormsg(ERR_INSERT_WRONG, -1, var, this->Filename);
                            }

                        }
                    } else {
                        _result += var;
                    }
                    var.clear();
                    _looking_for_var = false;
                } else {
                    _looking_for_var = true;
                }
            }
            continue;
        }
        _result += line[i];
    }
    line = _result;
}
void ATRCFiledata::AddBlock(const std::string& blockname){}
void ATRCFiledata::RemoveBlock(const std::string& blockname){}
void ATRCFiledata::AddVariable(const std::string& varname, const std::string& value){}
void ATRCFiledata::RemoveVariable(const std::string& varname){}
void ATRCFiledata::ModifyVariable(const std::string& varname, const std::string& value){}
void ATRCFiledata::AddKey(const std::string& block, const std::string& key, const std::string& value){}
void ATRCFiledata::RemoveKey(const std::string& block, const std::string& key){}
void ATRCFiledata::ModifyKey(const std::string& block, const std::string& key, const std::string& value){}



void _W_AddVariable_(PATRC_FD filedata, const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(_W_DoesExistVariable_(filedata, varname)){
        errormsg(ERR_VAR_EXISTS, -1, var.Name, filedata->Filename);
        return;
    }
    var.Value = value;
    filedata->Variables->push_back(var);
    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::ADD_VAR, -1, "%"+var.Name+"%="+ParseLineSTRINGtoATRC(var.Value));
    }
}
void _W_RemoveVariable_(PATRC_FD filedata, const std::string& varname){
    Variable var;
    var.Name = varname;
    if(!_W_DoesExistVariable_(filedata, varname)){
        errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, filedata->Filename);
        return;
    }

    int i = 0;    
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            break;
        }
        i++;
    }
    filedata->Variables->erase(filedata->Variables->begin() + i);

    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::REMOVE_VAR, i, varname);
    }
}
void _W_ModifyVariable_(PATRC_FD filedata, const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(!_W_DoesExistVariable_(filedata, varname)){
        errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, filedata->Filename);
        return;
    }
    int i = 0;    
    for(Variable &var : *filedata->Variables){
        if(var.Name == varname){
            break;
        }
        i++;
    }
    filedata->Variables->at(i).Value = value;
    // std::cout << "1." << filedata->Variables->at(i).Name +"="+ filedata->Variables->at(i).Value + "\n";
    // std::cout << "2." << varname +"="+ value + "\n";
    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::MODIFY_VAR, i, ParseLineSTRINGtoATRC(value)); // Values taken from filedata->Variables->at(xtra_info).[type]
    }
}



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






void _W_AddBlock_(PATRC_FD filedata, const std::string& blockname){
    Block block;
    block.Name = blockname;
    if(BlockContainsBlock(filedata->Blocks, block)) {
        errormsg(ERR_BLOCK_EXISTS, -1, block.Name, filedata->Filename);
        return;
    }
    filedata->Blocks->push_back(block);
    block.KeyCount++;
    if(filedata->AutoSave) {
        _W_Save_(filedata, ATRC_SAVE::ADD_BLOCK, -1, blockname);
    }
}
void _W_RemoveBlock_(PATRC_FD filedata, const std::string& blockname){
    Block block;
    block.Name = blockname;
    if(!BlockContainsBlock(filedata->Blocks, block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, blockname, filedata->Filename);
        return;
    }

    int i = 0;
    for(Block blk : *filedata->Blocks) {
        if(blk.Name == block.Name) break;
        i++;
    }
    filedata->Blocks->erase(filedata->Blocks->begin() + i);
    block.KeyCount--;
    if(filedata->AutoSave) {
        _W_Save_(filedata, ATRC_SAVE::REMOVE_BLOCK, i, blockname);
    }
}












bool BlockContainsKey(std::vector<Key>& keys, const Key &key) {
    for (const Key& _key : keys) {
        if (_key.Name == key.Name) {
            return true;
        }
    }
    return false;
}
bool BlockContainsBlock(std::unique_ptr<std::vector<Block>>& blocks, const Block &block) {
    for (Block &_block : *blocks) {
        if (_block.Name == block.Name) {
            return true;
        }
    }
    return false;
}
bool VariableContainsVariable(std::unique_ptr<std::vector<Variable>>& variables, const Variable &variable){
    for (Variable &var : *variables) {
        if (var.Name == variable.Name) {
            return true;
        }
    }
    return false;
}