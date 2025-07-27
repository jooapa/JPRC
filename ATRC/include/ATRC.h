/*+++
Header file for the ATRC (Advanced Tagged Resource Configuration Library) library.
  This file contains the declarations for the ATRC library, which provides functionality
for reading, writing, and manipulating configuration files in a structured format.
  This library is designed to be used in both C and C++ projects, with a focus on
ease of use and flexibility.

This file is part of the ATRC library project.
Author(s): Antonako1.

Licensed under the BSD 2-Clause License.
See the LICENSE file in the project root for license information.
---*/
#pragma once
#ifndef ATRC_H
#define ATRC_H
#if defined(_WIN32) || defined(_WIN64) || defined(WINDOWS_EXPORT_ALL_SYMBOLS)
#   ifdef ATRC_EXPORTS
#       define ATRC_API __declspec(dllexport)
#       define _WRAPPER_EXIM_ __declspec(dllexport)
#   else // ATRC_EXPORTS
#       define ATRC_API __declspec(dllimport)
#       define _WRAPPER_EXIM_ __declspec(dllimport)
#   endif // ATRC_EXPORTS
#endif // defined(_WIN32) || defined(_WIN64) || defined(WINDOWS_EXPORT_ALL_SYMBOLS)

#ifndef ATRC_API
#   define ATRC_API 
#endif // ATRC_API
#ifndef _WRAPPER_EXIM_
#   define _WRAPPER_EXIM_ 
#endif // _WRAPPER_EXIM_

#define FILEHEADER ""
#define __atrc__one__of__two__(cpp, c) \
    __atrc__one__of__two_helper__(cpp, c)
#define __nmsp__    namespace atrc {
#define __nmspe__   }
#ifdef __cplusplus
    #define __atrc__one__of__two_helper__(cpp, c) cpp
#else
    #define __atrc__one__of__two_helper__(cpp, c) c
#endif


#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#  include <vector>
#  include <string>
#  include <memory>
#  include <iostream>
namespace atrc {
// Disable warning C4251 for std::vector and std::string.
#  if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable: 4251)
#  endif // (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
#endif // __cplusplus


/*+++
C declarations
---*/
#ifdef __cplusplus
} // namespace atrc
extern "C" {
#endif // __cplusplus

typedef enum _ReadMode {
    ATRC_READ_ONLY,
    ATRC_CREATE_READ,
    ATRC_FORCE_READ,
} ReadMode;


typedef struct _C_Variable {
    char *Name;
    char *Value;
    bool IsPublic;
    uint64_t line_number;
} C_Variable, *C_PVariable;

typedef struct _C_Variable_Arr {
    C_Variable *Variables;
    uint64_t VariableCount;
} C_Variable_Arr, *C_PVariable_Arr;

typedef struct _C_Key {
    char *Name;
    char *Value;
    uint64_t line_number;
    uint64_t enum_value;
} C_Key, *C_PKey;

typedef struct _C_Block {
    char *Name;
    C_Key *Keys;
    uint64_t KeyCount;
    uint64_t line_number;
} C_Block, *C_PBlock;

typedef struct _C_Block_Arr {
    C_Block *Blocks;
    uint64_t BlockCount;
} C_Block_Arr, *C_PBlock_Arr;



typedef struct _ATRCFiledata{
    C_PVariable_Arr Variables;
    C_PBlock_Arr Blocks;
    char *Filename;
    bool AutoSave;
    bool Writecheck;
} C_ATRC_FD, *C_PATRC_FD;

ATRC_API bool Read(C_PATRC_FD self, const char* path, ReadMode readMode);
ATRC_API const char* ReadVariable(C_PATRC_FD self, const char* varname);
ATRC_API const char* ReadKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool DoesExistBlock(C_PATRC_FD self, const char* block);
ATRC_API bool DoesExistVariable(C_PATRC_FD self, const char* varname);
ATRC_API bool DoesExistKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool IsPublic(C_PATRC_FD self, const char* varname);
ATRC_API char* InsertVar_S(const char* line, const char** args);
ATRC_API bool AddBlock(C_PATRC_FD self, const char* blockname);
ATRC_API bool RemoveBlock(C_PATRC_FD self, const char* blockname);
ATRC_API bool AddVariable(C_PATRC_FD self, const char* varname, const char* value);
ATRC_API bool RemoveVariable(C_PATRC_FD self, const char* varname);
ATRC_API bool ModifyVariable(C_PATRC_FD self, const char* varname, const char* value);
ATRC_API bool AddKey(C_PATRC_FD self, const char* block, const char* key, const char* value);
ATRC_API bool RemoveKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool ModifyKey(C_PATRC_FD self, const char* block, const char* key, const char* value);
ATRC_API bool WriteCommentToBottom(C_PATRC_FD self, const char* comment);
ATRC_API bool WriteCommentToTop(C_PATRC_FD self, const char* comment);
ATRC_API uint64_t GetEnumValue(C_PATRC_FD self, const char* block, const char* key);

ATRC_API C_PATRC_FD Create_ATRC_FD(char *filename, ReadMode readMode);
ATRC_API C_PATRC_FD Create_Empty_ATRC_FD();
ATRC_API void *Destroy_ATRC_FD_Blocks_And_Keys(C_PATRC_FD self);
ATRC_API void *Destroy_ATRC_FD_Variables(C_PATRC_FD self);
ATRC_API void *Destroy_ATRC_FD(C_PATRC_FD self);

ATRC_API void *ATRC_FREE_MEMORY(void *ptr);

#ifdef __cplusplus
} // extern "C"
namespace atrc {
#endif // __cplusplus


/*+++
C++ only declarations
---*/
#ifdef __cplusplus
typedef struct ATRC_API _Variable {
    std::string Name;
    std::string Value;
    bool IsPublic = true;
    uint64_t line_number = (uint64_t)-1;
} Variable, * PVariable;

typedef struct ATRC_API _Key {
    std::string Name;
    std::string Value;
    uint64_t line_number = (uint64_t)-1;
    uint64_t enum_value = (uint64_t)-1;
} Key, * PKey;


typedef struct ATRC_API _Block {
    std::string Name;
    std::vector<Key> Keys;
    uint64_t line_number = (uint64_t)-1;
} Block, * PBlock;

class ATRC_API PROXY_ATRC_FD;

class ATRC_API ATRC_FD {
public:
    ATRC_FD();
    ATRC_FD(std::string& path, ReadMode mode = ATRC_READ_ONLY);
    ATRC_FD(const char *path, ReadMode mode = ATRC_READ_ONLY);
    ATRC_FD(C_PATRC_FD filedata);
    ~ATRC_FD();
    bool ReadAgain(ReadMode mode = ATRC_READ_ONLY);
    bool Read(std::string& path, ReadMode mode = ATRC_READ_ONLY);
    std::string ReadVariable(const std::string& varname);
    std::string ReadKey(const std::string& block, const std::string& key);
    bool DoesExistBlock(const std::string& block);
    bool DoesExistVariable(const std::string& varname);
    bool DoesExistKey(const std::string& block, const std::string& key);
    bool IsPublic(const std::string& varname);
    void InsertVar(std::string& line, std::vector<std::string>& args);
    std::string InsertVar_S(const std::string& line, std::vector<std::string>& args);
    bool AddBlock(const std::string& blockname);
    bool RemoveBlock(const std::string& blockname);
    bool AddVariable(const std::string& varname, const std::string& value);
    bool RemoveVariable(const std::string& varname);
    bool ModifyVariable(const std::string& varname, const std::string& value);
    bool AddKey(const std::string& block, const std::string& key, const std::string& value);
    bool RemoveKey(const std::string& block, const std::string& key);
    bool ModifyKey(const std::string& block, const std::string& key, const std::string& value);
    bool WriteCommentToTop(const std::string& comment);
    bool WriteCommentToBottom(const std::string& comment);
    C_PATRC_FD ToCStruct();

    uint64_t GetEnumValue(const std::string& block, const std::string& key);

    bool CheckStatus();

    std::vector<atrc::Variable> GetVariables();
    std::vector<Block>* GetBlocks();
    std::string GetFilename();
    bool GetAutoSave() const;
    void SetAutoSave(bool autosave);
    bool GetWriteCheck() const;
    void SetWriteCheck(bool writecheck);
    
    PROXY_ATRC_FD operator[](const std::string& key);
    PROXY_ATRC_FD operator[](const std::string& key) const;

private:
    void MAINCONSTRUCTOR();
    bool AutoSave;
    bool Writecheck;
    std::vector<Variable> Variables;
    std::vector<Block> Blocks;
    std::string Filename;
    bool safeToUse;
};
typedef ATRC_FD* PATRC_FD;

class ATRC_API PROXY_ATRC_FD {
public:
    PROXY_ATRC_FD(ATRC_FD& fd, const std::string& key);
    PROXY_ATRC_FD operator[](const std::string& subKey);
    operator std::string() const;
    operator const char*() const;
    PROXY_ATRC_FD& operator=(const std::string& value);    
    PROXY_ATRC_FD& operator>>(const std::string& value);
    PROXY_ATRC_FD& operator>>(const char* value);

	inline friend std::ostream& operator<<(std::ostream& os, const PROXY_ATRC_FD& obj) {
        uint64_t x = obj.key.find("]");
        if(x == std::string::npos) os << obj.fd->ReadVariable(obj.key);
        else {
            std::string block = obj.key.substr(0, x);
            std::string key_ = obj.key.substr(x + 1, obj.key.size() - x - 1);
            os << obj.fd->ReadKey(block, key_);
        }
		return os;
	}
private:
    PATRC_FD fd;
    std::string key;
};

#endif // __cplusplus




/*+++
Start of ATRC standard library
Namespace: atrc_std
---*/

#ifdef __cplusplus
} // namespace atrc
namespace atrc_std {
#endif // __cplusplus
/*
Inject macro for C
USAGE:
{
    const char *res = NULL;
    ATRC_CINJECT("INPUT %*%, %*%\n", "arg1", "arg2", ...) // -> "INPUT arg1, arg2\n"
    // use res as needed
    free(res);
}
*/
#define ATRC_CINJECT(line, ...) \
if(line != NULL) { \
    const char *message = line; \
    const char *args[] = {__VA_ARGS__, NULL}; \
    res = InsertVar_S(message, args); \
} \
else { \
    res = NULL; \
} \

#ifdef __cplusplus
/*
Inject macro for C++
USAGE:
{
    std::string res;
    ATRC_INJECT("INPUT %*%, %*%\n", "arg1", "arg2", ...) // -> "INPUT arg1, arg2\n"
    // use res as needed
}
*/
#define ATRC_INJECT(fd, line, ...) \
if(line != "") { \
    std::string message = line; \
    std::vector<std::string> args = {__VA_ARGS__}; \
    res = fd.InsertVar_S(message, args); \
} \
else { \
    res = ""; \
} 
#endif // __cplusplus

/*+++
ATRC STDLIB:
    - atrc_to_vector    (C++ only)
    - atrc_to_list      (C & C++)
    - atrc_free_list    (C & C++)
    - atrc_to_bool      (C & C++)
    - atrc_to_uint64_t  (C & C++)
    - atrc_to_int64_t   (C & C++)
    - atrc_to_double    (C & C++)
---*/

enum ATRC_ERR {
	_ATRC_SUCCESSFULL_ACTION = 0,
	_ATRC_UNSUCCESSFULL_ACTION = 1
};

extern uint64_t atrc_stdlib_errval;
extern bool atrc_stdlib_writecheck;

#ifdef __cplusplus
ATRC_API std::vector<std::string> atrc_to_vector(char separator, const std::string &value);
extern "C" {
#endif // __cplusplus

typedef struct _C_String_Arr {
    char **list;
    uint64_t count;
} C_String_Arr, *C_PString_Arr;

ATRC_API C_PString_Arr atrc_to_list(const char separator, const char* value);

ATRC_API void *atrc_free_list(C_PString_Arr list);

ATRC_API bool atrc_to_bool(const char* value);

ATRC_API uint64_t atrc_to_uint64_t(const char* value);

ATRC_API int64_t atrc_to_int64_t(const char* value);

ATRC_API double atrc_to_double(const char* value);

ATRC_API double atrc_math_exp(const char* value);

#ifdef __cplusplus
#   if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
#       pragma warning(pop)
#   endif // (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
    } // extern "C"
} // namespace atrc_std
#endif // __cplusplus

#endif // ATRC_H