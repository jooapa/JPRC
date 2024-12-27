#include "./include/DEV_INC.hpp"
#include "./include/filer.h"
#include <iostream>

void ATRCFiledata::MAINCONSTRUCTOR(){
    this->AutoSave = false;
    this->Filename = "";
    this->Variables = std::make_unique<std::vector<Variable>>(); 
    this->Blocks = std::make_unique<std::vector<Block>>();

}
ATRCFiledata::ATRCFiledata(PATRC_FD filedata){
    this->MAINCONSTRUCTOR();
    
    this->Filename = filedata->Filename;
    std::vector<Variable> vars;
    for(int i = 0; i < sizeof(filedata->Variables); i++){
        Variable var;
        var.Name = filedata->Variables[i].Name;
        var.Value = filedata->Variables[i].Value;
        var.IsPublic = filedata->Variables[i].IsPublic;
        vars.push_back(var);
    }
    this->Variables = std::make_unique<std::vector<Variable>>(vars);

    std::vector<Block> blocks;
    for(int i = 0; i < sizeof(filedata->Blocks); i++){
        Block block;
        block.Name = filedata->Blocks[i].Name;
        for(int j = 0; j < sizeof(filedata->Blocks[i].Keys); j++){
            Key key;
            key.Name = filedata->Blocks[i].Keys[j].Name;
            key.Value = filedata->Blocks[i].Keys[j].Value;
            block.Keys.push_back(key);
        }
        blocks.push_back(block);
    }
    this->Blocks = std::make_unique<std::vector<Block>>(blocks);

    this->AutoSave = filedata->AutoSave;
}
ATRCFiledata::ATRCFiledata(){this->MAINCONSTRUCTOR();}
ATRCFiledata::ATRCFiledata(const char *path){
    this->MAINCONSTRUCTOR();
    this->Filename = path;
    this->Read();
}
ATRCFiledata::~ATRCFiledata(){
    this->Variables->clear();
    this->Blocks->clear();
    this->Variables.reset();
    this->Blocks.reset();
}
bool ATRCFiledata::Read(){
    std::string filename = this->Filename;
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
    auto parsedData = ParseFile(filename, encoding, extension);
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << filename << std::endl;
        return false;
    } else {
        this->Variables = std::move(parsedData.first);
        this->Blocks = std::move(parsedData.second);
    }
    return true;
}

PATRC_FD ATRCFiledata::ToCStruct(){
    PATRC_FD filedata = CreateEmptyFiledata();
	if (filedata == nullptr) return nullptr;
    filedata->Filename = this->Filename.c_str();
    C_Variable *vars = (C_Variable*)malloc(sizeof(C_Variable) * this->Variables->size());
    if (vars == nullptr)
    {
		DestroyFiledata(&filedata);
        return nullptr;
    }
    for(int i = 0; i < this->Variables->size(); i++){
        vars[i].Name = this->Variables->at(i).Name.c_str();
        vars[i].Value = this->Variables->at(i).Value.c_str();
        vars[i].IsPublic = this->Variables->at(i).IsPublic;
    }
    filedata->Variables = vars;

    C_Block *blocks = (C_Block*)malloc(sizeof(C_Block) * this->Blocks->size());
    if (blocks == nullptr) {
		free(vars);
		DestroyFiledata(&filedata);
        return nullptr;
    }
    for(int i = 0; i < this->Blocks->size(); i++){
        blocks[i].Name = this->Blocks->at(i).Name.c_str();
        blocks[i].KeyCount = this->Blocks->at(i).Keys.size();
        C_Key *keys = (C_Key*)malloc(sizeof(C_Key) * this->Blocks->at(i).Keys.size());
        if (keys == nullptr) {
			free(vars);
			free(blocks);
			DestroyFiledata(&filedata);
			return nullptr;
        }
        for(int j = 0; j < this->Blocks->at(i).Keys.size(); j++){
            keys[j].Name = this->Blocks->at(i).Keys[j].Name.c_str();
            keys[j].Value = this->Blocks->at(i).Keys[j].Value.c_str();
        }
        blocks[i].Keys = keys;
    }
    filedata->Blocks = blocks;
    return filedata;
}


std::vector<Variable>* ATRCFiledata::GetVariables(){
    return this->Variables.get();
}
std::vector<Block>* ATRCFiledata::GetBlocks(){
    return this->Blocks.get();
}
std::string ATRCFiledata::GetFilename(){
    return this->Filename;
}
bool ATRCFiledata::GetAutoSave(){
    return this->AutoSave;
}
void ATRCFiledata::SetAutoSave(bool autosave){
    this->AutoSave = autosave;
}