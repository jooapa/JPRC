#pragma once
#ifndef ATRC_CPP_HPP
#define ATRC_CPP_HPP
#include <ATRC_COMMON.hpp>
#include <ATRC_C.h>
#ifdef __cplusplus

namespace atrc
{

typedef struct Variable {
    std::string Name;
    std::string Value;
    bool IsPublic;
} Variable, *PVariable;

typedef struct Key {
    std::string Name;
    std::string Value;
} Key, *PKey;


typedef struct Block {
    std::string Name;
    std::vector<Key> Keys;
} Block, *PBlock;

    
class ATRCFiledata{
public:
    ATRCFiledata();
    ATRCFiledata(const char *path);
    ATRCFiledata(PATRC_FD filedata);
    ~ATRCFiledata(); 
    bool Read();
    std::string ReadVariable(const std::string& varname);
    std::string ReadKey(const std::string& block, const std::string& key);
    bool DoesExistBlock(const std::string& block);
    bool DoesExistVariable(const std::string& varname);
    bool DoesExistKey(const std::string& block, const std::string& key);
    bool IsPublic(const std::string& varname);
    void InsertVar(std::string &line, std::vector<std::string> &args);
    std::string InsertVar_S(const std::string &line, std::vector<std::string> &args);
    void AddBlock(const std::string& blockname);
    void RemoveBlock(const std::string& blockname);
    void AddVariable(const std::string& varname, const std::string& value);
    void RemoveVariable(const std::string& varname);
    void ModifyVariable(const std::string& varname, const std::string& value);
    void AddKey(const std::string& block, const std::string& key, const std::string& value);
    void RemoveKey(const std::string& block, const std::string& key);
    void ModifyKey(const std::string& block, const std::string& key, const std::string& value);
    PATRC_FD ToCStruct();
private:
    void MAINCONSTRUCTOR();
    std::unique_ptr<std::vector<Variable>> Variables;
    std::unique_ptr<std::vector<Block>> Blocks;
    std::string Filename;
    bool AutoSave;
};

#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
#  pragma warning(pop)
#endif // _WIN32 || _WIN64
} // namespace atrc
#endif // __cplusplus
#endif // ATRC_CPP_HPP
