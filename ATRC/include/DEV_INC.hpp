#pragma once
#include <ATRC_COMMON.hpp>
#include <ATRC_CPP.hpp>
#include <ATRC_C.h>
#include <ATRC_UTILS.h>
#include <ATRC.h>
using namespace atrc;

// File
#define ERR_CLASS_FILEHANDLER           100

// Error types
#define ERR_INVALID_VAR_DECL            101
#define ERR_INVALID_BLOCK_DECL          102
#define ERR_INVALID_KEY_DECL            103
#define ERR_NO_VAR_VECTOR               104
#define ERR_REREFERENCED_VAR            105
#define ERR_REREFERENCED_BLOCK          106
#define ERR_REREFERENCED_KEY            107
#define ERR_INSERT_VAR                  108  
#define ERR_INVALID_FILE                109

// File
#define ERR_CLASS_READER                200

// Error types
#define ERR_UNAUTHORIZED_ACCESS_TO_VAR  201

// File
#define ERR_CLASS_SAVER                 300

// Error types
#define ERR_BLOCK_NOT_FOUND             301
#define ERR_KEY_NOT_FOUND               302
#define ERR_VAR_NOT_FOUND               303
#define ERR_BLOCK_EXISTS                304
#define ERR_KEY_EXISTS                  305
#define ERR_VAR_EXISTS                  306
#define ERR_INSERT_WRONG                307

// File
#define ERR_CLASS_STDLIB                401

// Error types
#define ERR_STDLIB_CAST_ERROR           402
