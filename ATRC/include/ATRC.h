#pragma once
#ifndef ATRC_H
#define ATRC_H
#define __MICROSOFT_CHECK__ defined(_WIN32) || defined(_WIN64) || defined(WINDOWS_EXPORT_ALL_SYMBOLS)
#define __MSVC_CHECK__      (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER) 
#ifdef ATRC_EXPORTS
#   if __MICROSOFT_CHECK__
#       define ATRC_API __declspec(dllexport)
#       define _WRAPPER_EXIM_ __declspec(dllexport)
#   endif // __MICROSOFT_CHECK__
#else
#   if __MICROSOFT_CHECK__
#       define ATRC_API __declspec(dllimport)
#       define _WRAPPER_EXIM_ __declspec(dllimport)
#   endif // __MICROSOFT_CHECK__
#endif // ATRC_EXPORTS

#ifndef ATRC_API
#   define ATRC_API 
#endif // ATRC_API
#ifndef _WRAPPER_EXIM_
#   define _WRAPPER_EXIM_ 
#endif // _WRAPPER_EXIM_

#define FILEHEADER "#__ATRC__\0"

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#define __atrc__one__of__two__(cpp, c) \
    __atrc__one__of__two_helper__(cpp, c)

#define __nmsp__    namespace atrc {
#define __nmspe__   }



#ifdef __cplusplus
    #define __atrc__one__of__two_helper__(cpp, c) cpp
#else
    #define __atrc__one__of__two_helper__(cpp, c) c
#endif


#ifdef __cplusplus
#  include <vector>
#  include <string>
#  include <memory>
#  include <ostream>
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


typedef struct ATRC_API C_Variable {
    const char *Name;
    const char *Value;
    bool IsPublic;
} C_Variable, *C_PVariable;

typedef struct ATRC_API _C_Variable_Arr {
    C_Variable *Variables;
    uint64_t VariableCount;
} C_Variable_Arr, *C_PVariable_Arr;

typedef struct ATRC_API C_Key {
    const char *Name;
    const char *Value;
} C_Key, *C_PKey;

typedef struct ATRC_API _C_Block {
    const char *Name;
    C_Key *Keys;
    uint64_t KeyCount;
} C_Block, *C_PBlock;

typedef struct ATRC_API _C_Block_Arr {
    C_Block *Blocks;
    uint64_t BlockCount;
} C_Block_Arr, *C_PBlock_Arr;



typedef struct ATRC_API _ATRCFiledata{
    C_PVariable_Arr Variables;
    C_PBlock_Arr Blocks;
    const char *Filename;
    bool AutoSave;
} C_ATRC_FD, *C_PATRC_FD;

ATRC_API bool Read(C_PATRC_FD self);
ATRC_API const char* ReadVariable(C_PATRC_FD self, const char* varname);
ATRC_API const char* ReadKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool DoesExistBlock(C_PATRC_FD self, const char* block);
ATRC_API bool DoesExistVariable(C_PATRC_FD self, const char* varname);
ATRC_API bool DoesExistKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API bool IsPublic(C_PATRC_FD self, const char* varname);
ATRC_API void InsertVar(C_PATRC_FD self, const char* line, const char** args);
ATRC_API const char* InsertVar_S(C_PATRC_FD self, const char* line, const char** args);
ATRC_API void AddBlock(C_PATRC_FD self, const char* blockname);
ATRC_API void RemoveBlock(C_PATRC_FD self, const char* blockname);
ATRC_API void AddVariable(C_PATRC_FD self, const char* varname, const char* value);
ATRC_API void RemoveVariable(C_PATRC_FD self, const char* varname);
ATRC_API void ModifyVariable(C_PATRC_FD self, const char* varname, const char* value);
ATRC_API void AddKey(C_PATRC_FD self, const char* block, const char* key, const char* value);
ATRC_API void RemoveKey(C_PATRC_FD self, const char* block, const char* key);
ATRC_API void ModifyKey(C_PATRC_FD self, const char* block, const char* key, const char* value);


ATRC_API C_PATRC_FD Create_ATRC_FD(const char *filename);
ATRC_API C_PATRC_FD Create_Empty_ATRC_FD();
ATRC_API void Destroy_ATRC_FD(C_PATRC_FD self);

bool _ATRC_WRAP_READ(C_PATRC_FD self);
void _ATRC_WRAP_ERRORMSG(int err_num, int line_number, const char *var_name, const char *filename);

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
        void AddBlock(const std::string& blockname);
        void RemoveBlock(const std::string& blockname);
        void AddVariable(const std::string& varname, const std::string& value);
        void RemoveVariable(const std::string& varname);
        void ModifyVariable(const std::string& varname, const std::string& value);
        void AddKey(const std::string& block, const std::string& key, const std::string& value);
        void RemoveKey(const std::string& block, const std::string& key);
        void ModifyKey(const std::string& block, const std::string& key, const std::string& value);
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
    
	friend std::ostream& operator<<(std::ostream& os, const PROXY_ATRC_FD& obj) {
        os << obj.fd->ReadVariable(obj.key);
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

typedef struct ATRC_API _C_String_Arr {
    char **list;
    uint64_t count;
} C_String_Arr, *C_PString_Arr;

ATRC_API C_String_Arr atrc_to_list(char separator, const char* value);

ATRC_API void atrc_free_list(C_String_Arr *list);

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