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

struct Variable {
    std::string Name;
    std::string Value;
    bool IsPublic;
};

struct Key {
    std::string Name;
    std::string Value;
};


struct Block {
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

ATRC_API std::shared_ptr<ATRC_FD> 
Read
(
    const std::string& filename = "", 
    const std::string& encoding = "utf-8", 
    const std::string& allowed_extension = ""
);

ATRC_API std::string 
ReadVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname
);

ATRC_API std::string 
ReadKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key
);

ATRC_API bool 
DoesExistBlock
(
    std::shared_ptr<ATRC_FD> filedata,
    const std::string& block
);

ATRC_API bool 
DoesExistVariable
(
    std::shared_ptr<ATRC_FD> filedata,
    const std::string& varname
);

ATRC_API bool 
DoesExistKey
(
    std::shared_ptr<ATRC_FD> filedata,
    const std::string& block, 
    const std::string& key
);

ATRC_API bool 
IsPublic
(
    std::shared_ptr<ATRC_FD> filedata,
    const std::string& varname
);

ATRC_API void 
InsertVar
(
    std::string &line,
    std::vector<std::string> &args, 
    std::shared_ptr<ATRC_FD> filedata
);

ATRC_API void 
AddBlock
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& blockname
);

ATRC_API void 
RemoveBlock
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& blockname
);

ATRC_API void 
AddVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname, 
    const std::string& value
);

ATRC_API void 
RemoveVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname
);

ATRC_API void 
ModifyVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname, 
    const std::string& value
);

ATRC_API void 
AddKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key, 
    const std::string& value
);

ATRC_API void 
RemoveKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key
);

ATRC_API void 
ModifyKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key, 
    const std::string& value
);


#ifdef INCLUDE_ATRC_STDLIB
#include <stdint.h>

#define     SUCCESSFULL_ACTION      1
#define     UNSUCCESFULL_ACTION     0

ATRC_API extern int atrc_stdlib_errval;

ATRC_API std::vector<std::string> 
atrc_to_list
(
    char separator, 
    const std::string &value
);

ATRC_API bool 
atrc_to_bool
(
    const std::string &value
);

ATRC_API uint64_t 
atrc_to_uint64_t
(
    const std::string &value
);

ATRC_API int64_t 
atrc_to_int64_t
(
    const std::string &value
);

ATRC_API double 
atrc_to_double
(
    const std::string &value
);

#endif // INCLUDE_ATRC_STDLIB

#ifdef _WIN32
#  pragma warning(pop)
#endif

#endif // ATRC_HPP
