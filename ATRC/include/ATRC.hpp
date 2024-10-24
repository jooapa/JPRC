#pragma once

#ifndef ATRC_HPP
#define ATRC_HPP

#ifdef ATRC_API
#  undef ATRC_API
#endif

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

#include <vector>
#include <string>
#include <memory>

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

typedef struct _ATRCFiledata{
    std::unique_ptr<std::vector<Variable>> Variables;
    std::unique_ptr<std::vector<Block>> Blocks;
    std::string Filename;
    std::string Extension;
    std::string Encoding;

    /// true to save a changes
    bool AutoSave = false;
} ATRC_FD, *PATRC_FD;

ATRC_API ATRC_FD* Read(const std::string& filename = "", const std::string& encoding = "utf-8", const std::string& allowed_extension = "");

ATRC_API std::string ReadVariable(ATRC_FD *filedata, const std::string& varname);

ATRC_API std::string ReadKey(ATRC_FD *filedata, const std::string& block, const std::string& key);

ATRC_API bool DoesExistBlock(ATRC_FD* filedata, const std::string& block);

ATRC_API bool DoesExistVariable(ATRC_FD* filedata, const std::string& varname);

ATRC_API bool DoesExistKey(ATRC_FD* filedata, const std::string& block, const std::string& key);

ATRC_API bool IsPublic(ATRC_FD* filedata, const std::string& varname);

ATRC_API void InsertVar(std::string &line, std::vector<std::string> &args, ATRC_FD *filedata);

ATRC_API void AddBlock(ATRC_FD *filedata, const std::string& blockname);

ATRC_API void RemoveBlock(ATRC_FD *filedata, const std::string& blockname);

ATRC_API void AddVariable(ATRC_FD *filedata, const std::string& varname, const std::string& value);

ATRC_API void RemoveVariable(ATRC_FD *filedata, const std::string& varname);

ATRC_API void ModifyVariable(ATRC_FD *filedata, const std::string& varname, const std::string& value);

ATRC_API void AddKey(ATRC_FD *filedata, const std::string& block, const std::string& key, const std::string& value);

ATRC_API void RemoveKey(ATRC_FD *filedata, const std::string& block, const std::string& key);

ATRC_API void ModifyKey(ATRC_FD *filedata, const std::string& block, const std::string& key, const std::string& value);

ATRC_API void DeleteATRCFiledata(ATRC_FD *filedata);

#ifdef INCLUDE_ATRC_STDLIB
#include <stdint.h>
#define     SUCCESSFULL_ACTION      1
#define     UNSUCCESFULL_ACTION     0
extern int  atrc_stdlib_errval;

ATRC_API std::vector<std::string> atrc_to_list(char separator, const std::string &value);

ATRC_API bool atrc_to_bool(const std::string &value);

ATRC_API uint64_t atrc_to_uint64_t(const std::string &value);

ATRC_API int64_t atrc_to_int64_t(const std::string &value);

ATRC_API float atrc_to_float(const std::string &value);

ATRC_API double atrc_to_double(const std::string &value);
#endif // INCLUDE_ATRC_STDLIB

#ifdef _WIN32
#  pragma warning(pop)
#endif

#endif // ATRC_HPP
