#pragma once

#ifndef ATRC_HPP
#define ATRC_HPP

#ifdef ATRC_API
#  undef ATRC_API
#endif

#ifdef _WIN32
    #ifdef ATRC_EXPORTS
    #  define ATRC_API __declspec(dllexport)
    #  define _WRAPPER_EXIM_ __declspec(dllexport)
    #else
    #  define ATRC_API __declspec(dllimport)
    #  define _WRAPPER_EXIM_ __declspec(dllimport)
    #endif
#else
    #define ATRC_API
    #define _WRAPPER_EXIM_
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

    /// true to save a changes
    bool AutoSave = false;

    _ATRCFiledata(const std::string &path);
    bool IRead();
    std::string IReadVariable(const std::string& varname);
    std::string IReadKey(const std::string& block, const std::string& key);
    bool IDoesExistBlock(const std::string& block);
    bool IDoesExistVariable(const std::string& varname);
    bool IDoesExistKey(const std::string& block, const std::string& key);
    bool IIsPublic(const std::string& varname);
    void IInsertVar(std::string &line, std::vector<std::string> &args);
    std::string IInsertVar_S(const std::string &line, std::vector<std::string> &args);
    void IAddBlock(const std::string& blockname);
    void IRemoveBlock(const std::string& blockname);
    void IAddVariable(const std::string& varname, const std::string& value);
    void IRemoveVariable(const std::string& varname);
    void IModifyVariable(const std::string& varname, const std::string& value);
    void IAddKey(const std::string& block, const std::string& key, const std::string& value);
    void IRemoveKey(const std::string& block, const std::string& key);
    void IModifyKey(const std::string& block, const std::string& key, const std::string& value);
} ATRC_FD, *PATRC_FD;

ATRC_API std::shared_ptr<ATRC_FD> 
Read
(
    const std::string& path = "" 
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
    const std::string &line,
    std::vector<std::string> &args, 
    std::shared_ptr<ATRC_FD> filedata
);

ATRC_API std::string 
InsertVar_S
(
    const std::string &line,
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

_WRAPPER_EXIM_ std::string _W_ReadVariable_(PATRC_FD filedata, const std::string& varname);
_WRAPPER_EXIM_ std::string _W_ReadKey_(PATRC_FD filedata, const std::string& block, const std::string& key);
_WRAPPER_EXIM_ bool _W_DoesExistBlock_(PATRC_FD filedata, const std::string& block);
_WRAPPER_EXIM_ bool _W_DoesExistVariable_(PATRC_FD filedata, const std::string& varname);
_WRAPPER_EXIM_ bool _W_DoesExistKey_(PATRC_FD filedata, const std::string& block, const std::string& key);
_WRAPPER_EXIM_ bool _W_IsPublic_(PATRC_FD filedata, const std::string& varname);
_WRAPPER_EXIM_ void _W_InsertVar_(PATRC_FD filedata, std::string &line, std::vector<std::string> &args);
_WRAPPER_EXIM_ std::string _W_InsertVar_S_(PATRC_FD filedata, const std::string &line, std::vector<std::string> &args);
_WRAPPER_EXIM_ void _W_AddBlock_(PATRC_FD filedata, const std::string& blockname);
_WRAPPER_EXIM_ void _W_RemoveBlock_(PATRC_FD filedata, const std::string& blockname);
_WRAPPER_EXIM_ void _W_AddVariable_(PATRC_FD filedata, const std::string& varname, const std::string& value);
_WRAPPER_EXIM_ void _W_RemoveVariable_(PATRC_FD filedata, const std::string& varname);
_WRAPPER_EXIM_ void _W_ModifyVariable_(PATRC_FD filedata, const std::string& varname, const std::string& value);
_WRAPPER_EXIM_ void _W_AddKey_(PATRC_FD filedata, const std::string& block, const std::string& key, const std::string& value);
_WRAPPER_EXIM_ void _W_RemoveKey_(PATRC_FD filedata, const std::string& block, const std::string& key);
_WRAPPER_EXIM_ void _W_ModifyKey_(PATRC_FD filedata, const std::string& block, const std::string& key, const std::string& value);

#ifdef _WIN32
#  pragma warning(pop)
#endif



#endif // ATRC_HPP
