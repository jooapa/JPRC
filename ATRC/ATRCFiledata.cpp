#include "./include/ATRC.hpp"
#include "./include/filer.h"


_ATRCFiledata::_ATRCFiledata(const std::string &path)
{
    Variables = std::make_unique<std::vector<Variable>>();
    Blocks = std::make_unique<std::vector<Block>>();
    Filename = path;

    this->IRead();
}

bool _ATRCFiledata::IRead(){
    std::string encoding = "UTF-8";
    std::string extension = "atrc";
    auto parsedData = ParseFile(Filename, encoding, extension);
    // Check for successful parsing
    if (parsedData.first->empty() && parsedData.second->empty()) {
        std::cerr << "Failed to parse file: " << Filename << std::endl;
    } else {
        Variables = std::move(parsedData.first);
        Blocks = std::move(parsedData.second);
    }
    return true;
}


std::string _ATRCFiledata::IReadVariable(const std::string& varname){
    return _W_ReadVariable_(this, varname);
}
std::string _ATRCFiledata::IReadKey(const std::string& block, const std::string& key){
    return _W_ReadKey_(this, block, key);
}
bool _ATRCFiledata::IDoesExistBlock(const std::string& block){
    return _W_DoesExistBlock_(this, block);
}
bool _ATRCFiledata::IDoesExistVariable(const std::string& varname){
    return _W_DoesExistVariable_(this, varname);
}
bool _ATRCFiledata::IDoesExistKey(const std::string& block, const std::string& key){
    return _W_DoesExistKey_(this, block, key);
}
bool _ATRCFiledata::IIsPublic(const std::string& varname){
    return _W_IsPublic_(this, varname);
}
void _ATRCFiledata::IInsertVar(std::string &line, std::vector<std::string> &args){
    return _W_InsertVar_(this, line, args);
}
std::string _ATRCFiledata::IInsertVar_S(const std::string &line, std::vector<std::string> &args){
    return _W_InsertVar_S_(this, line, args);
}
void _ATRCFiledata::IAddBlock(const std::string& blockname){
    return _W_AddBlock_(this, blockname);
}
void _ATRCFiledata::IRemoveBlock(const std::string& blockname){
    return _W_RemoveBlock_(this, blockname);
}
void _ATRCFiledata::IAddVariable(const std::string& varname, const std::string& value){
    return _W_AddVariable_(this, varname, value);
}
void _ATRCFiledata::IRemoveVariable(const std::string& varname){
    return _W_RemoveVariable_(this, varname);
}
void _ATRCFiledata::IModifyVariable(const std::string& varname, const std::string& value){
    return _W_ModifyVariable_(this, varname, value);
}
void _ATRCFiledata::IAddKey(const std::string& block, const std::string& key, const std::string& value){
    return _W_AddKey_(this, block, key, value);
}
void _ATRCFiledata::IRemoveKey(const std::string& block, const std::string& key){
    return _W_RemoveKey_(this, block, key);
}
void _ATRCFiledata::IModifyKey(const std::string& block, const std::string& key, const std::string& value){
    return _W_ModifyKey_(this, block, key, value);
}