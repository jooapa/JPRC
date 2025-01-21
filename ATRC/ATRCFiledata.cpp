#define _CRT_SECURE_NO_DEPRECATE
#include <ATRC.h>
#include <filer.h>
#include <iostream>
#include <cstring>
#include <fstream>

void atrc::ATRC_FD::MAINCONSTRUCTOR(){
    this->AutoSave = false;
    this->Writecheck = false;
    this->Filename = "";
    this->Variables = std::make_unique<std::vector<Variable>>(); 
    this->Blocks = std::make_unique<std::vector<Block>>();

}

atrc::ATRC_FD::ATRC_FD(){this->MAINCONSTRUCTOR();}
atrc::ATRC_FD::ATRC_FD(const char *path, ReadMode mode){
    this->MAINCONSTRUCTOR();
    this->Filename = path;
    this->ReadAgain(mode);
}

atrc::ATRC_FD::ATRC_FD(C_PATRC_FD fd){
    this->MAINCONSTRUCTOR();
    this->AutoSave = fd->AutoSave;
    this->Filename = fd->Filename;
    this->Variables = std::make_unique<std::vector<Variable>>();
    this->Blocks = std::make_unique<std::vector<Block>>();
    for(uint64_t i = 0; i < fd->Variables->VariableCount; i++){
        Variable var;
        var.Name = fd->Variables->Variables[i].Name;
        var.Value = fd->Variables->Variables[i].Value;
        var.IsPublic = fd->Variables->Variables[i].IsPublic;
        this->Variables->push_back(var);
    }
    for(uint64_t i = 0; i < fd->Blocks->BlockCount; i++){
        Block blk;
        blk.Name = fd->Blocks->Blocks[i].Name;
        for(uint64_t j = 0; j < fd->Blocks->Blocks[i].KeyCount; j++){
            Key key;
            key.Name = fd->Blocks->Blocks[i].Keys[j].Name;
            key.Value = fd->Blocks->Blocks[i].Keys[j].Value;
            blk.Keys.push_back(key);
        }
        this->Blocks->push_back(blk);
    }
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


bool atrc::ATRC_FD::Read(const char* path, ReadMode mode){
    this->Filename = path;
    return this->ReadAgain(mode);
}

bool atrc::ATRC_FD::ReadAgain(ReadMode mode){
    std::string filename = this->Filename;
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
    if (mode == ATRC_FORCE_READ) {
        // Delete previous file and create a new one
        std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
        if (ofs.is_open()) {
            ofs << FILEHEADER << "\n";
        } else {
            atrc::errormsg(FILE_MODE_ERR, -1, filename, filename);
            return false;
        }
    } else if (mode == ATRC_CREATE_READ) {
        std::ifstream ifs(filename);
        if (!ifs.good()) {
            std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
            if (ofs.is_open()) {
                ofs << FILEHEADER << "\n";
            } else {
                atrc::errormsg(FILE_MODE_ERR, -1, filename, filename);
                return false;
            }
        }
    }

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

C_PATRC_FD atrc::ATRC_FD::ToCStruct() {
    C_PATRC_FD self = Create_Empty_ATRC_FD();
    if (!self) return NULL;

    self->AutoSave = this->AutoSave;

    // Allocate and copy Filename
    self->Filename = new char[this->Filename.size() + 1];
    if (!self->Filename) {
        Destroy_ATRC_FD(self);
        return NULL;
    }
    std::strcpy(self->Filename, this->Filename.c_str());

    // Allocate memory for Variables
    self->Variables->VariableCount = 0;
    self->Variables->Variables = (C_Variable*)malloc(this->Variables->size() * sizeof(C_Variable));
    if (!self->Variables->Variables) {
        Destroy_ATRC_FD(self);
        return NULL;
    }

    for (const atrc::Variable& var : *this->Variables) {
        // Allocate Name
        self->Variables->Variables[self->Variables->VariableCount].Name = new char[var.Name.size() + 1];
        if (!self->Variables->Variables[self->Variables->VariableCount].Name) {
            Destroy_ATRC_FD(self);
            return NULL;
        }
        std::strcpy(self->Variables->Variables[self->Variables->VariableCount].Name, var.Name.c_str());

        // Allocate Value
        self->Variables->Variables[self->Variables->VariableCount].Value = new char[var.Value.size() + 1];
        if (!self->Variables->Variables[self->Variables->VariableCount].Value) {
            Destroy_ATRC_FD(self);
            return NULL;
        }
        std::strcpy(self->Variables->Variables[self->Variables->VariableCount].Value, var.Value.c_str());

        // Copy IsPublic
        self->Variables->Variables[self->Variables->VariableCount].IsPublic = var.IsPublic;
        self->Variables->VariableCount++;
    }

    // Allocate memory for Blocks
    self->Blocks->BlockCount = 0;
    self->Blocks->Blocks = (C_Block*)malloc(this->Blocks->size() * sizeof(C_Block));
    if (!self->Blocks->Blocks) {
        Destroy_ATRC_FD(self);
        return NULL;
    }

    for (const atrc::Block& block : *this->Blocks) {
        // Allocate Block Name
        self->Blocks->Blocks[self->Blocks->BlockCount].Name = new char[block.Name.size() + 1];
        if (!self->Blocks->Blocks[self->Blocks->BlockCount].Name) {
            Destroy_ATRC_FD(self);
            return NULL;
        }
        std::strcpy(self->Blocks->Blocks[self->Blocks->BlockCount].Name, block.Name.c_str());

        // Allocate Keys
        self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount = 0;
        self->Blocks->Blocks[self->Blocks->BlockCount].Keys = (C_Key*)malloc(block.Keys.size() * sizeof(C_Key));
        if (!self->Blocks->Blocks[self->Blocks->BlockCount].Keys) {
            Destroy_ATRC_FD(self);
            return NULL;
        }

        for (const atrc::Key& key : block.Keys) {
            // Allocate Key Name
            self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Name = new char[key.Name.size() + 1];
            if (!self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Name) {
                Destroy_ATRC_FD(self);
                return NULL;
            }
            std::strcpy(self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Name, key.Name.c_str());

            // Allocate Key Value
            self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Value = new char[key.Value.size() + 1];
            if (!self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Value) {
                Destroy_ATRC_FD(self);
                return NULL;
            }
            std::strcpy(self->Blocks->Blocks[self->Blocks->BlockCount].Keys[self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount].Value, key.Value.c_str());

            self->Blocks->Blocks[self->Blocks->BlockCount].KeyCount++;
        }

        self->Blocks->BlockCount++;
    }

    return self;
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

bool atrc::ATRC_FD::GetWriteCheck() const {
    return this->Writecheck;
}
void atrc::ATRC_FD::SetWriteCheck(bool writecheck) {
    this->Writecheck = writecheck;
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
bool atrc::ATRC_FD::AddBlock(const std::string& blockname){
    if(DoesExistBlock(blockname)){
        errormsg(ERR_BLOCK_EXISTS, -1, blockname, this->Filename);
        return false;
    }
    Block blk;
    blk.Name = blockname;
    this->Blocks->push_back(blk);
    if(this->AutoSave){
        atrc::_W_Save_(this, atrc::ATRC_SAVE::ADD_BLOCK, -1, blockname);
    }
    return true;
}
bool atrc::ATRC_FD::RemoveBlock(const std::string& blockname){
    Block block;
    block.Name = blockname;
    if(!BlockContainsBlock(this->Blocks, block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, blockname, this->Filename);
        return false;
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
    return true;
}
bool atrc::ATRC_FD::AddVariable(const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(DoesExistVariable(varname)){
        errormsg(ERR_VAR_EXISTS, -1, var.Name, this->Filename);
        return false;
    }
    var.Value = value;
    this->Variables->push_back(var);
    if(this->AutoSave){
        atrc::_W_Save_(this, atrc::ATRC_SAVE::ADD_VAR, -1, "%"+var.Name+"%="+atrc::ParseLineSTRINGtoATRC(var.Value));
    }
    return true;
}
bool atrc::ATRC_FD::RemoveVariable(const std::string& varname){
    Variable var;
    var.Name = varname;
    if(!DoesExistVariable(varname)){
        errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, this->Filename);
        return false;
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
    return true;
}
bool atrc::ATRC_FD::ModifyVariable(const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(!DoesExistVariable(varname)){
        if(!this->Writecheck) {
            errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, this->Filename);
            return false;
        }
        if(!this->AddVariable(varname, value)){
            errormsg(ERR_WRITECHECK, -1, var.Name, this->Filename);
            return false;
        }
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
    return true;
}
bool atrc::ATRC_FD::AddKey(const std::string& block, const std::string& key, const std::string& value){
    if(!DoesExistBlock(block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, this->Filename);
        return false;
    }
    if(DoesExistKey(block, key)){
        errormsg(ERR_KEY_EXISTS, -1, key, this->Filename);
        return false;
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
    return true;
}
bool atrc::ATRC_FD::RemoveKey(const std::string& block, const std::string& key){
    if(!DoesExistBlock(block)) {
        errormsg(ERR_BLOCK_NOT_FOUND, -1, block, this->Filename);
        return false;
    }
    if(!DoesExistKey(block, key)){
        errormsg(ERR_KEY_NOT_FOUND, -1, key, this->Filename);
        return false;
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
    return true;
}
bool atrc::ATRC_FD::ModifyKey(const std::string& block, const std::string& key, const std::string& value){
    if(!DoesExistBlock(block)) {
        if(!this->Writecheck){
            errormsg(ERR_BLOCK_NOT_FOUND, -1, block, this->Filename);
            return false;
        } else {
            if(!this->AddBlock(block)){
                errormsg(ERR_WRITECHECK, -1, block, this->Filename);
                return false;
            }
        }
    }
    if(!DoesExistKey(block, key)){
        if(!this->Writecheck){
            errormsg(ERR_KEY_NOT_FOUND, -1, key, this->Filename);
            return false;
        } else {
            if(!this->AddKey(block, key, value)){
                errormsg(ERR_WRITECHECK, -1, key, this->Filename);
                return false;
            }
        }
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
    return true;
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

atrc::PROXY_ATRC_FD::operator const char*() const {
    uint64_t x = key.find("]");
    try {
        if (x == std::string::npos) {
            std::string res_str = fd->ReadVariable(key);
            const char *res = new char[res_str.size() + 1];
            std::strcpy(const_cast<char*>(res), res_str.c_str());
            return res;
        }
        else {
            std::string block = key.substr(0, x);
            std::string key_ = key.substr(x + 1, key.size() - x - 1);
            std::string res_str = fd->ReadKey(block, key_);
            const char *res = new char[res_str.size() + 1];
            std::strcpy(const_cast<char*>(res), res_str.c_str());
            return res;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return "";
}

atrc::PROXY_ATRC_FD::operator std::string() const {
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
            if(fd->GetWriteCheck() || fd->DoesExistVariable(key)) {
                fd->ModifyVariable(key, value);
            }
        }
        else {
            std::string block = key.substr(0, x);
            std::string key_ = key.substr(x + 1, key.size() - x - 1);
            if(fd->GetWriteCheck() || fd->DoesExistKey(block, key_)) {
                    fd->ModifyKey(block, key_, value);
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return *this;
}


atrc::PROXY_ATRC_FD& atrc::PROXY_ATRC_FD::operator>>(const std::string& value) {
    uint64_t x = key.find("]");
    if (x == std::string::npos) {
        std::string existing = fd->ReadVariable(key);
        fd->ModifyVariable(key, existing + value);
    } else {
        std::string block = key.substr(0, x);
        std::string key_ = key.substr(x + 1, key.size() - x - 1);
        std::string existing = fd->ReadKey(block, key_);
        fd->ModifyKey(block, key_, existing + value);
    }
    return *this;
}

atrc::PROXY_ATRC_FD& atrc::PROXY_ATRC_FD::operator>>(const char* value) {
    return operator>>(std::string(value));
}