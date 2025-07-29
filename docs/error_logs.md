# Error logs

Error logs are defined as shown below

```cpp
"~ATRC~Error<" + err_class + "?" + err_num + ">" + "<" + filename + ">"+ ": " + msg
// Outputs "~ATRC~ERROR~<100?105><test.atrc>: Re-Rereferenced variable: 'testvar' at line 4"
```

- `err_class` denotes which part of the library logged the message
- `err_num` denotes the error message
- `filename` denotes the filename where the error occured in
- `msg` denotes the error message

Thus the example shown above tells that the error occured inside 
filehandler, where a rereferenced variable was found inside "test.atrc", by the name "testvar" at line 4

Most of the time the error class is paired to error number, but 
there can be times when this not followed

```h
#define ERR_CLASS_FILEHANDLER           100 // File parser
#define ERR_CLASS_READER                200 // File reader
#define ERR_CLASS_SAVER                 300 // File saver
#define ERR_CLASS_STDLIB                401 // Standard library
#define ERR_CLASS_MEMORY                501 // Memory error

#define ERR_INVALID_VAR_DECL            101 // Invalid variable declaration
#define ERR_INVALID_BLOCK_DECL          102 // Invalid block declaration
#define ERR_INVALID_KEY_DECL            103 // Invalid key declaration
#define ERR_NO_VAR_VECTOR               104 // Parser found variable reference inside a value, but no value was saved by that name
#define ERR_REREFERENCED_VAR            105 // Parser found another occurance of an already variable
#define ERR_REREFERENCED_BLOCK          106 // Parser found another occurance of an already block
#define ERR_REREFERENCED_KEY            107 // Parser found another occurance of an already key
#define ERR_INSERT_VAR                  108 // Invalid variable insert marker found
#define ERR_INVALID_FILE                109 // Error with file. Either with the path or fileheader
#define FILE_MODE_ERR                   110 // Error with file creating when using ATRC_FORCE_READ or ATRC_CREATE_READ
#define ERR_WRITECHECK                  111 // Writecheck failed when modifying a key or variable
#define ERR_INVALID_PREPROCESSOR_FLAG   112 // Preprocessor flag is invalid
#define ERR_INVALID_PREPROCESSOR_TAG    113 // Preprocessor tag is invalid
#define ERR_INVALID_PREPROCESSOR_VALUE  114 // Preprocessor value is invalid
#define ERR_INVALID_PREPROCESSOR_SYNTAX 115 // Preprocessor syntax is invalid
#define ERR_INVALID_SAVE_ACTION         116 // _W_Save got invalid save action
#define ERR_INVALID_PREPROCESSOR_FLAG_CONTENTS 117 // Invalid flag contents inside a preprocessor flag
#define ERR_INVALID_INCLUDE_FILE        118 // Invalid include file. Not found
#define ERR_INVALID_RAW_STRING          119 // Raw string failure
#define ERR_UNAUTHORIZED_ACCESS_TO_VAR  201 // Unauthorized access to private variable
#define ERR_BLOCK_NOT_FOUND             301 // Block was not found
#define ERR_KEY_NOT_FOUND               302 // Key was not found
#define ERR_VAR_NOT_FOUND               303 // Variable was not found
#define ERR_BLOCK_EXISTS                304 // Block does not exist
#define ERR_KEY_EXISTS                  305 // Variable does not exist
#define ERR_VAR_EXISTS                  306 // Variable does not exist
#define ERR_INSERT_WRONG                307 // Invalid insert variable declaration
#define ERR_EMPTY_COMMENT               308 // Unused
#define ERR_STDLIB_CAST_ERROR           402 // Error when converting a string into boolean, double or integer with stdlib
#define ERR_INVALID_POWER_VALUE         403 // Invalid value for power with stdlibs math_exp function
#define ERR_INVALID_EXPRESSION          404 // Invalid math expression
#define ERR_INVALID_NUMBER              405 // Invalid number
#define ERR_MEMORY_ALLOCATION_FAILED    502 // Memory allocation failed
#define ERR_NULL_POINTER				503 // Null pointer error 
```