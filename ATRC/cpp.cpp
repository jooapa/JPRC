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
		if (vars == NULL) return false;
        for(int i = 0; i < parsedData.first->size(); i++){
            vars[i].Name = parsedData.first->at(i).Name.c_str();
            vars[i].Value = parsedData.first->at(i).Value.c_str();
            vars[i].IsPublic = parsedData.first->at(i).IsPublic;
        }
        self->Variables = vars;

        C_Block *blocks = (C_Block*)malloc(sizeof(C_Block) * parsedData.second->size());
		if (blocks == nullptr) {
			free(vars);
			return false;
		}
        for(int i = 0; i < parsedData.second->size(); i++){
			blocks[i].Name = parsedData.second->at(i).Name.c_str();
            blocks[i].KeyCount = parsedData.second->at(i).Keys.size();
            C_Key *keys = (C_Key*)malloc(sizeof(C_Key) * parsedData.second->at(i).Keys.size());
			if (keys == nullptr) {
				free(vars);
				free(blocks);
				return false;
			}
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
void ATRCFiledata::AddBlock(const std::string& blockname){
    if(DoesExistBlock(blockname)){
        errormsg(ERR_BLOCK_EXISTS, -1, blockname, this->Filename);
        return;
    }
    Block blk;
    blk.Name = blockname;
    this->Blocks->push_back(blk);
    if(this->AutoSave){
        _W_Save_(this, ATRC_SAVE::ADD_BLOCK, -1, blockname);
    }
    
}
void ATRCFiledata::RemoveBlock(const std::string& blockname){
    Block block;
    block.Name = blockname;
    if(!BlockContainsBlock(this->Blocks, block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, blockname, this->Filename);
        return;
    }

    int i = 0;
    for(Block blk : *this->Blocks) {
        if(blk.Name == block.Name) break;
        i++;
    }
    this->Blocks->erase(this->Blocks->begin() + i);
    if(this->AutoSave) {
        _W_Save_(this, ATRC_SAVE::REMOVE_BLOCK, i, blockname);
    }
}
void ATRCFiledata::AddVariable(const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(DoesExistVariable(varname)){
        errormsg(ERR_VAR_EXISTS, -1, var.Name, this->Filename);
        return;
    }
    var.Value = value;
    this->Variables->push_back(var);
    if(this->AutoSave){
        _W_Save_(this, ATRC_SAVE::ADD_VAR, -1, "%"+var.Name+"%="+ParseLineSTRINGtoATRC(var.Value));
    }
}
void ATRCFiledata::RemoveVariable(const std::string& varname){
    Variable var;
    var.Name = varname;
    if(!DoesExistVariable(varname)){
        errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, this->Filename);
        return;
    }

    int i = 0;    
    for(Variable var : *this->Variables){
        if(var.Name == varname){
            break;
        }
        i++;
    }
    this->Variables->erase(this->Variables->begin() + i);

    if(this->AutoSave){
        _W_Save_(this, ATRC_SAVE::REMOVE_VAR, i, varname);
    }
}
void ATRCFiledata::ModifyVariable(const std::string& varname, const std::string& value){
Variable var;
    var.Name = varname;
    if(!DoesExistVariable(varname)){
        errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, this->Filename);
        return;
    }
    int i = 0;    
    for(Variable &var : *this->Variables){
        if(var.Name == varname){
            break;
        }
        i++;
    }
    this->Variables->at(i).Value = value;
    if(this->AutoSave){
        _W_Save_(this, ATRC_SAVE::MODIFY_VAR, i, ParseLineSTRINGtoATRC(value)); // Values taken from filedata->Variables->at(xtra_info).[type]
    }
}
void ATRCFiledata::AddKey(const std::string& block, const std::string& key, const std::string& value){
    if(!DoesExistBlock(block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, this->Filename);
        return;
    }
    if(DoesExistKey(block, key)){
        errormsg(ERR_KEY_EXISTS, -1, key, this->Filename);
        return;
    }

    Key _key;
    _key.Name = key;
    _key.Value = value;
    for(Block &_block : *this->Blocks){
        if(_block.Name == block){
            _block.Keys.push_back(_key);
            break;
        }
    }
    if(this->AutoSave){
        _W_Save_(this, ATRC_SAVE::ADD_KEY, -1, key, value, block);
    }
}
void ATRCFiledata::RemoveKey(const std::string& block, const std::string& key){
    if(!DoesExistBlock(block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, this->Filename);
        return;
    }
    if(!DoesExistKey(block, key)){
        errormsg(ERR_KEY_NOT_FOUND, -1, key, this->Filename);
        return;
    }
    int i = 0;
    for (Block &_block : *this->Blocks) {
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


    if(this->AutoSave){
        _W_Save_(this, ATRC_SAVE::REMOVE_KEY, i, key, "", block);
    }
}
void ATRCFiledata::ModifyKey(const std::string& block, const std::string& key, const std::string& value){
    if(!DoesExistBlock(block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, this->Filename);
        return;
    }
    if(!DoesExistKey(block, key)){
        errormsg(ERR_KEY_NOT_FOUND, -1, key, this->Filename);
        return;
    }

    int i = 0;
    for(Block &_blk : *this->Blocks){
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

    if(this->AutoSave){
        _W_Save_(this, ATRC_SAVE::MODIFY_KEY, i, key, value, block);
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


void _ATRC_WRAP_ERRORMSG(int err_num, int line_number, const char *var_name, const char *filename){
    errormsg(err_num, line_number, var_name, filename);
}