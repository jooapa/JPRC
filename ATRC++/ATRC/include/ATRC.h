#ifndef ATRC_H
#define ATRC_H

#include <vector>
#include <string>
#include <memory>

#ifdef _WIN32
    #ifdef ATRC_EXPORTS
    #  define ATRC_API __declspec(dllexport)
    #else
    #  define ATRC_API __declspec(dllimport)
    #endif
#else
    #define ATRC_API
#endif

// Disable warning C4251 for std::vector and std::string
#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable: 4251)
#endif

struct ATRC_API Variable {
    std::string Name;
    std::string Value;
    bool IsPublic;
};

struct ATRC_API Key {
    std::string Name;
    std::string Value;
};

struct ATRC_API Block {
    std::string Name;
    std::vector<Key> Keys;
};

struct ATRC_API ATRCFiledata {
    std::unique_ptr<std::vector<Variable>> Variables;
    std::unique_ptr<std::vector<Block>> Blocks;
    std::string Filename;
    std::string Encoding;
    bool AutoSave = false;
};


/// @brief Reads file.atrc file
/// @param filename filename to read
/// @param encoding 'utf-8' for now
/// @return Pointer to ATRCFiledata struct
ATRC_API std::unique_ptr<ATRCFiledata> Read(std::string& filename, const std::string& encoding);

/// @brief Read a variable to a string
/// @param filedata Filedata struct
/// @param varname Variable name to read
/// @param contents Contents where to store the variable
/// @return 
ATRC_API void ReadVariable(ATRCFiledata *filedata, const std::string& varname, std::string& contents);

/// @brief Read a key from a block to a string
/// @param filedata Filedata struct
/// @param block Block where to read from
/// @param key Key to read
/// @param contents Contents where to store the key
/// @return 
ATRC_API void ReadKey(ATRCFiledata *filedata, const std::string& block, const std::string& key, std::string& contents);

/// @brief Check if a block exists
/// @param filedata Filedata struct
/// @param block Block to check
/// @return true if block exists, false otherwise
ATRC_API bool DoesExistBlock(ATRCFiledata* filedata, const std::string& block);

/// @brief Check if a variable exists
/// @param filedata Filedata struct
/// @param varname Variable name to check
/// @return true if variable exists, false otherwise
ATRC_API bool DoesExistVariable(ATRCFiledata* filedata, const std::string& varname);

/// @brief Check if a key exists in a block
/// @param filedata Filedata struct
/// @param block Block to check
/// @param key Key to check
/// @return true if key exists, false otherwise
ATRC_API bool DoesExistKey(ATRCFiledata* filedata, const std::string& block, const std::string& key);

/// @brief Check if a variable is public
/// @param filedata Filedata struct
/// @param varname Variable name to check
/// @return true if variable is public, false otherwise
ATRC_API bool IsPublic(ATRCFiledata* filedata, const std::string& varname);

/// @brief Insert variables into a string
/// @param line Line to insert variables into
/// @param args Variables to insert into the line, [&arg1, &arg2, ...]
/// @param filedata 
ATRC_API void InsertVar(std::string &line, std::vector<std::string> &args, ATRCFiledata *filedata);

/// @brief Add a block to the filedata
/// @param filedata Filedata struct
/// @param blockname Block name to add
/// @returns
ATRC_API void AddBlock(ATRCFiledata *filedata, const std::string& blockname);

/// @brief Remove a block from the filedata
/// @param filedata Filedata struct
/// @param blockname Block name to remove
/// @returns
ATRC_API void RemoveBlock(ATRCFiledata *filedata, const std::string& blockname);

/// @brief Add a variable to the filedata
/// @param filedata Filedata struct
/// @param varname Variable name to add
/// @param value Value of the variable
/// @returns
ATRC_API void AddVariable(ATRCFiledata *filedata, const std::string& varname, const std::string& value);

/// @brief Remove a variable from the filedata
/// @param filedata Filedata struct
/// @param varname Variable name to remove
/// @returns
ATRC_API void RemoveVariable(ATRCFiledata *filedata, const std::string& varname);

/// @brief Modify a variable in the filedata
/// @param filedata Filedata struct
/// @param varname Variable name to modify
/// @param value New value of the variable
/// @returns
ATRC_API void ModifyVariable(ATRCFiledata *filedata, const std::string& varname, const std::string& value);

/// @brief Add a key to a block
/// @param filedata Filedata struct
/// @param block Block to add the key to
/// @param key Key to add
/// @param value Value of the key
/// @returns
ATRC_API void AddKey(ATRCFiledata *filedata, const std::string& block, const std::string& key, const std::string& value);

/// @brief Remove a key from a block
/// @param filedata Filedata struct
/// @param block Block to remove the key from
/// @param key Key to remove
/// @returns
ATRC_API void RemoveKey(ATRCFiledata *filedata, const std::string& block, const std::string& key);

/// @brief Modify a key in a block
/// @param filedata Filedata struct
/// @param block Block to modify
/// @param key Key to modify
/// @param value New value of the key
/// @returns
ATRC_API void ModifyKey(ATRCFiledata *filedata, const std::string& block, const std::string& key, const std::string& value);

#ifdef _WIN32
#  pragma warning(pop)
#endif

#endif // ATRC_H
