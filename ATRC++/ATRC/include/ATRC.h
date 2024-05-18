#ifndef ATRC_H
#define ATRC_H

#ifdef __cplusplus
#include <vector>
#include <string>
#endif

#ifdef ATRC_EXPORTS
#  define ATRC_API __declspec(dllexport)
#else
#  define ATRC_API __declspec(dllimport)
#endif

// Disable warning C4251 for std::vector and std::string
#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable: 4251)
#endif

#ifdef __cplusplus
class ATRC_API Variable {
public:
    std::string Name;
    std::string Value;
    bool IsPublic;
};

class ATRC_API Key {
public:
    std::string Name;
    std::string Value;
};

class ATRC_API Block {
public:
    std::string Name;
    std::vector<Key> Keys;
};

class ATRC_API ATRCFiledata {
private:
    std::vector<Variable> variables;
public:
    std::vector<Variable> *Variables;
    std::vector<Block> *Blocks;
    std::string Filename;
    std::string Encoding;

    ATRCFiledata();
    ~ATRCFiledata();
};

extern "C" {
ATRC_API ATRCFiledata* Read(const std::string& filename, const std::string& encoding);
}

#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <wchar.h>
#define INSERTVAR_MAX 1024
/// @brief Insert variables into a string
/// @param line Line to insert variables into
/// @param args Variables to insert into the line, end with NULL
ATRC_API void InsertVar(wchar_t *line, const wchar_t *args[]);
#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#  pragma warning(pop)
#endif

#endif // ATRC_H
