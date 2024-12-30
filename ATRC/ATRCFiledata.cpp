#include <ATRC.h>
#include <filer.h>
#include <iostream>



void atrc::ATRC_FD::MAINCONSTRUCTOR(){
    this->AutoSave = false;
    this->Filename = "";
    this->Variables = std::make_unique<std::vector<Variable>>(); 
    this->Blocks = std::make_unique<std::vector<Block>>();

}
atrc::ATRC_FD::ATRC_FD(C_PATRC_FD filedata){
    this->MAINCONSTRUCTOR();
    std::unique_ptr<std::vector<Variable>> vars = std::make_unique<std::vector<Variable>>();
    std::unique_ptr<std::vector<Block>> blks = std::make_unique<std::vector<Block>>();
    this->AutoSave = filedata->AutoSave;
    this->Filename = filedata->Filename;

    this->Variables = std::move(vars);
    this->Blocks = std::move(blks);
}
atrc::ATRC_FD::ATRC_FD(){this->MAINCONSTRUCTOR();}
atrc::ATRC_FD::ATRC_FD(const char *path){
    this->MAINCONSTRUCTOR();
    this->Filename = path;
    this->Read();
}
atrc::ATRC_FD::~ATRC_FD(){
    this->Variables->clear();
    this->Blocks->clear();
    this->Variables.reset();
    this->Blocks.reset();
}

atrc::PROXY_ATRC_FD atrc::ATRC_FD::operator[](const std::string& key) {
    return atrc::PROXY_ATRC_FD(*this, key);
}

atrc::PROXY_ATRC_FD atrc::ATRC_FD::operator[](const std::string& key) const {
    return atrc::PROXY_ATRC_FD(const_cast<atrc::ATRC_FD&>(*this), key);
}



bool atrc::ATRC_FD::Read(){
    std::string filename = this->Filename;
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
    auto parsedData = atrc::ParseFile(filename, encoding, extension);
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
        return false;
    } else {
        this->Variables = std::move(parsedData.first);
        this->Blocks = std::move(parsedData.second);
    }
    return true;
}

C_PATRC_FD atrc::ATRC_FD::ToCStruct(){
    C_PATRC_FD filedata = Create_Empty_ATRC_FD();
    filedata->AutoSave = this->AutoSave;
    filedata->Filename = this->Filename.c_str();
    
    
    return filedata;
}

std::vector<atrc::Variable>* atrc::ATRC_FD::GetVariables(){
    return this->Variables.get();
}
std::vector<atrc::Block>* atrc::ATRC_FD::GetBlocks(){
    return this->Blocks.get();
}
std::string atrc::ATRC_FD::GetFilename(){
    return this->Filename;
}
bool atrc::ATRC_FD::GetAutoSave() const {
    return this->AutoSave;
}
void atrc::ATRC_FD::SetAutoSave(bool autosave){
    this->AutoSave = autosave;
}

std::string atrc::ATRC_FD::ReadVariable(const std::string& varname){
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
std::string atrc::ATRC_FD::ReadKey(const std::string& block, const std::string& key){
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
std::string atrc::ATRC_FD::InsertVar_S(const std::string &line, std::vector<std::string> &args){
    std::string res = line;
    InsertVar(res, args);
    return res;
}
bool atrc::ATRC_FD::DoesExistBlock(const std::string& block){
    for(Block &blk : *this->Blocks){
        if(blk.Name == block) return true;
    }
    return false;
}
bool atrc::ATRC_FD::DoesExistVariable(const std::string& varname){
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

bool atrc::ATRC_FD::CheckStatus() {
	if (Variables->empty() && Blocks->empty()) return false;
    return true;
}

bool atrc::ATRC_FD::DoesExistKey(const std::string& block, const std::string& key){
    for(Block &blk : *this->Blocks){
            if(blk.Name == block){
                for(Key &k : blk.Keys){
                    if(k.Name == key) return true;
                }
            }
        }
        return false;
}
bool atrc::ATRC_FD::IsPublic(const std::string& varname){
    for(Variable &var : *this->Variables){
        if(var.Name == varname){
            return var.IsPublic;
        } 
    }
    return false;
}
void atrc::ATRC_FD::InsertVar(std::string &line, std::vector<std::string> &args){
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
                            _result += args[(size_t)_arg_counter++];
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
void atrc::ATRC_FD::AddBlock(const std::string& blockname){
    if(DoesExistBlock(blockname)){
        errormsg(ERR_BLOCK_EXISTS, -1, blockname, this->Filename);
        return;
    }
    Block blk;
    blk.Name = blockname;
    this->Blocks->push_back(blk);
    if(this->AutoSave){
        atrc::_W_Save_(this, atrc::ATRC_SAVE::ADD_BLOCK, -1, blockname);
    }
    
}
void atrc::ATRC_FD::RemoveBlock(const std::string& blockname){
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
        atrc::_W_Save_(this, atrc::ATRC_SAVE::REMOVE_BLOCK, i, blockname);
    }
}
void atrc::ATRC_FD::AddVariable(const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(DoesExistVariable(varname)){
        errormsg(ERR_VAR_EXISTS, -1, var.Name, this->Filename);
        return;
    }
    var.Value = value;
    this->Variables->push_back(var);
    if(this->AutoSave){
        atrc::_W_Save_(this, atrc::ATRC_SAVE::ADD_VAR, -1, "%"+var.Name+"%="+atrc::ParseLineSTRINGtoATRC(var.Value));
    }
}
void atrc::ATRC_FD::RemoveVariable(const std::string& varname){
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
        atrc::_W_Save_(this, atrc::ATRC_SAVE::REMOVE_VAR, i, varname);
    }
}
void atrc::ATRC_FD::ModifyVariable(const std::string& varname, const std::string& value){
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
    this->Variables->at((size_t)i).Value = value;
    if(this->AutoSave){
        atrc::_W_Save_(this, atrc::ATRC_SAVE::MODIFY_VAR, i, atrc::ParseLineSTRINGtoATRC(value)); // Values taken from filedata->Variables->at(xtra_info).[type]
    }
}
void atrc::ATRC_FD::AddKey(const std::string& block, const std::string& key, const std::string& value){
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
        atrc::_W_Save_(this, atrc::ATRC_SAVE::ADD_KEY, -1, key, value, block);
    }
}
void atrc::ATRC_FD::RemoveKey(const std::string& block, const std::string& key){
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
        atrc::_W_Save_(this, atrc::ATRC_SAVE::REMOVE_KEY, i, key, "", block);
    }
}
void atrc::ATRC_FD::ModifyKey(const std::string& block, const std::string& key, const std::string& value){
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
        atrc::_W_Save_(this, atrc::ATRC_SAVE::MODIFY_KEY, i, key, value, block);
    }
}




/*+++
PROXY_ATRC_FD
---*/
#include <iostream>
atrc::PROXY_ATRC_FD::PROXY_ATRC_FD(atrc::ATRC_FD& fd, const std::string& key) : fd(&fd), key(key) {}

atrc::PROXY_ATRC_FD atrc::PROXY_ATRC_FD::operator[](const std::string& subKey) {
	std::string combined_key = key + "]" + subKey;
	return atrc::PROXY_ATRC_FD(*fd, combined_key);
}

atrc::PROXY_ATRC_FD::operator std::string() const {
	std::cout << key << std::endl;
	uint64_t x = key.find("]");
    try {
		if (x == std::string::npos) {
			return fd->ReadVariable(key);
		}
		else {
			std::string block = key.substr(0, x);
			std::string key_ = key.substr(x + 1, key.size() - x - 1);
			return fd->ReadKey(block, key_);
		}
	}
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return "";
}
atrc::PROXY_ATRC_FD& atrc::PROXY_ATRC_FD::operator=(const std::string& value) {
    uint64_t x = key.find("]");
    try {
        if (x == std::string::npos) {
            if(!fd->DoesExistVariable(key)){
                fd->AddVariable(key, value);
            } else {
                fd->ModifyVariable(key, value);
            }
        }
        else {
            std::string block = key.substr(0, x);
            std::string key_ = key.substr(x + 1, key.size() - x - 1);
            if(!fd->DoesExistBlock(block)){
                fd->AddBlock(block);
                fd->AddKey(block, key_, value);
            } else {
                if(!fd->DoesExistKey(block, key_)){
                    fd->AddKey(block, key_, value);
                } else {
                    fd->ModifyKey(block, key_, value);
                }
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return *this;
}
