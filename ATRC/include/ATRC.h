#pragma once
#ifndef ATRC_H
#define ATRC_H
#define __MICROSOFT_CHECK__     defined(_WIN32) || defined(_WIN64) || defined(WINDOWS_EXPORT_ALL_SYMBOLS)
#define __MSVC_CHECK__          (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER) 

#if __MICROSOFT_CHECK__ || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__) || defined(_WIN32) || defined(_WIN64)
#   ifdef ATRC_EXPORTS
#       define ATRC_API __declspec(dllexport)
#       define _WRAPPER_EXIM_ __declspec(dllexport)
#   else // ATRC_EXPORTS
#       define ATRC_API __declspec(dllimport)
#       define _WRAPPER_EXIM_ __declspec(dllimport)
#   endif // ATRC_EXPORTS
#endif // __MICROSOFT_CHECK__

#ifndef ATRC_API
#   define ATRC_API 
#endif // ATRC_API
#ifndef _WRAPPER_EXIM_
#   define _WRAPPER_EXIM_ 
#endif // _WRAPPER_EXIM_

#define FILEHEADER "#!ATRC\0"
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
#include <limits.h>
#ifdef __cplusplus
#  include <vector>
#  include <string>
#  include <memory>
#  include <iostream>
namespace atrc {
// Disable warning C4251 for std::vector and std::string.
#  if __MSVC_CHECK__
#    pragma warning(push)
#    pragma warning(disable: 4251)
#  endif // __MSVC_CHECK__
#endif // __cplusplus


/*+++
C declarations
---*/
#ifdef __cplusplus
} // namespace atrc
extern "C" {
#endif // __cplusplus


typedef struct C_Variable {
    char *Name;
    char *Value;
    bool IsPublic;
} C_Variable, *C_PVariable;

typedef struct _C_Variable_Arr {
    C_Variable *Variables;
    uint64_t VariableCount;
} C_Variable_Arr, *C_PVariable_Arr;

typedef struct C_Key {
    char *Name;
    char *Value;
} C_Key, *C_PKey;

typedef struct _C_Block {
    char *Name;
    C_Key *Keys;
    uint64_t KeyCount;
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
} C_ATRC_FD, *C_PATRC_FD;

ATRC_API bool Read(C_PATRC_FD self, const char* path);
ATRC_API const char* ReadVariable(C_PATRC_FD self, const char* varname);
ATRC_API const char* ReadKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool DoesExistBlock(C_PATRC_FD self, const char* block);
ATRC_API bool DoesExistVariable(C_PATRC_FD self, const char* varname);
ATRC_API bool DoesExistKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool IsPublic(C_PATRC_FD self, const char* varname);
ATRC_API const char* InsertVar_S(const char* line, const char** args);
ATRC_API bool AddBlock(C_PATRC_FD self, const char* blockname);
ATRC_API bool RemoveBlock(C_PATRC_FD self, const char* blockname);
ATRC_API bool AddVariable(C_PATRC_FD self, const char* varname, const char* value);
ATRC_API bool RemoveVariable(C_PATRC_FD self, const char* varname);
ATRC_API bool ModifyVariable(C_PATRC_FD self, const char* varname, const char* value);
ATRC_API bool AddKey(C_PATRC_FD self, const char* block, const char* key, const char* value);
ATRC_API bool RemoveKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool ModifyKey(C_PATRC_FD self, const char* block, const char* key, const char* value);


ATRC_API C_PATRC_FD Create_ATRC_FD(char *filename);
ATRC_API C_PATRC_FD Create_Empty_ATRC_FD();
ATRC_API void Destroy_ATRC_FD_Blocks_And_Keys(C_PATRC_FD self);
ATRC_API void Destroy_ATRC_FD_Variables(C_PATRC_FD self);
ATRC_API void Destroy_ATRC_FD(C_PATRC_FD self);

#ifdef __cplusplus
} // extern "C"
namespace atrc {
#endif // __cplusplus


/*+++
C++ only declarations
---*/
#ifdef __cplusplus
typedef struct ATRC_API Variable {
    std::string Name;
    std::string Value;
    bool IsPublic = true;
} Variable, * PVariable;

typedef struct ATRC_API Key {
    std::string Name;
    std::string Value;
} Key, * PKey;


typedef struct ATRC_API _Block {
    std::string Name;
    std::vector<Key> Keys;
} Block, * PBlock;

class ATRC_API PROXY_ATRC_FD;

class ATRC_API ATRC_FD {
    public:
        ATRC_FD();
        ATRC_FD(const char* path);
        ATRC_FD(C_PATRC_FD filedata);
        ~ATRC_FD();
        bool Read();
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
        C_PATRC_FD ToCStruct();

        bool CheckStatus();

        std::vector<Variable>* GetVariables();
        std::vector<Block>* GetBlocks();
        std::string GetFilename();
        bool GetAutoSave() const;
        void SetAutoSave(bool autosave);

        PROXY_ATRC_FD operator[](const std::string& key);
        PROXY_ATRC_FD operator[](const std::string& key) const;

    private:
        void MAINCONSTRUCTOR();
        bool AutoSave;
        std::unique_ptr<std::vector<Variable>> Variables;
        std::unique_ptr<std::vector<Block>> Blocks;
        std::string Filename;
    };
typedef ATRC_FD* PATRC_FD;

class ATRC_API PROXY_ATRC_FD {
public:
    PROXY_ATRC_FD(ATRC_FD& fd, const std::string& key);
    PROXY_ATRC_FD operator[](const std::string& subKey);
    operator std::string() const;
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

#ifdef __cplusplus
ATRC_API std::vector<std::string> atrc_to_vector(char separator, const std::string &value);
extern "C" {
#endif // __cplusplus

typedef struct _C_String_Arr {
    char **list;
    uint64_t count;
} C_String_Arr, *C_PString_Arr;

ATRC_API C_PString_Arr atrc_to_list(const char separator, const char* value);

ATRC_API void atrc_free_list(C_PString_Arr list);

ATRC_API bool atrc_to_bool(const char* value);

ATRC_API uint64_t atrc_to_uint64_t(const char* value);

ATRC_API int64_t atrc_to_int64_t(const char* value);

ATRC_API double atrc_to_double(const char* value);


#ifdef __cplusplus
#   if __MSVC_CHECK__
#       pragma warning(pop)
#   endif // __MSVC_CHECK__
    } // extern "C"
} // namespace atrc
#endif // __cplusplus

#endif // ATRC_H