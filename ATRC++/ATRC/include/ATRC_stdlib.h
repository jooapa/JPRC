#ifndef ATRC_STDLIB_H
#define ATRC_STDLIB_H
#include <ATRC.h>

#include <vector>
#include <string>

/*+++
    If you want to use your own function on errors, use the designated defines for them
    
    Boolean conversion:
        #define ATRC_BOOLEAN_CONVERSION (*function_ptr)()
---*/
#define ATRC_GLOBAL_ERROR_FUNCTION

/// @brief Turn an ATRC value to a string vector
/// @param separator Separator character used in the value
/// @param value ATRC value
/// @return Value as a list
ATRC_API std::vector<std::string> atrc_value_to_list(const char &separator, const std::string &value);

/// @brief Turns an ATRC value (string) to boolean
/// @param value ATRC value
/// @return True if string is true, false if string is false
ATRC_API bool   atrc_value_to_bool(const std::string &value);

/// @brief Turns an ATRC value (string) to size_t
/// @param value ATRC value
/// @return 
ATRC_API size_t atrc_value_to_size_t(const std::string &value);

/// @brief Turns an ATRC value (string) to int
/// @param value ATRC value
/// @return 
ATRC_API int    atrc_value_to_int(const std::string &value);

/// @brief Turns an ATRC value (string) to float
/// @param value ATRC value
/// @return 
ATRC_API float  atrc_value_to_float(const std::string &value);

#endif //ATRC_STDLIB_H