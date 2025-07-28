# ATRC C API Documentation

## Table of Contents

- [Enums](#enums)
  - [ReadMode](#readmode)
- [Data Structures](#data-structures)
  - [C_Variable](#c_variable)
  - [C_Variable_Arr](#c_variable_arr)
  - [C_Key](#c_key)
  - [C_Block](#c_block)
  - [C_Block_Arr](#c_block_arr)
  - [C_ATRC_FD](#c_atrc_fd)
- [File Operations](#file-operations)
  - [Read](#read)
- [Data Access Functions](#data-access-functions)
  - [ReadVariable](#readvariable)
  - [ReadKey](#readkey)
- [Existence Check Functions](#existence-check-functions)
  - [DoesExistBlock](#doesexistblock)
  - [DoesExistVariable](#doesexistvariable)
  - [DoesExistKey](#doesexistkey)
  - [IsPublic](#ispublic)
- [Variable Injection](#variable-injection)
  - [InsertVar_S](#insertvar_s)
- [Block Management](#block-management)
  - [AddBlock](#addblock)
  - [RemoveBlock](#removeblock)
- [Variable Management](#variable-management)
  - [AddVariable](#addvariable)
  - [RemoveVariable](#removevariable)
  - [ModifyVariable](#modifyvariable)
- [Key Management](#key-management)
  - [AddKey](#addkey)
  - [RemoveKey](#removekey)
  - [ModifyKey](#modifykey)
- [Comment Functions](#comment-functions)
  - [WriteCommentToBottom](#writecommenttobottom)
  - [WriteCommentToTop](#writecommenttotop)
- [Enum Functions](#enum-functions)
  - [GetEnumValue](#getenumvalue)
- [Constructor/Destructor Functions](#constructordestructor-functions)
  - [Create_ATRC_FD](#create_atrc_fd)
  - [Create_Empty_ATRC_FD](#create_empty_atrc_fd)
  - [Destroy_ATRC_FD_Blocks_And_Keys](#destroy_atrc_fd_blocks_and_keys)
  - [Destroy_ATRC_FD_Variables](#destroy_atrc_fd_variables)
  - [Destroy_ATRC_FD](#destroy_atrc_fd)
- [Memory Management](#memory-management)
  - [ATRC_FREE_MEMORY](#atrc_free_memory)

---

## Enums

### ReadMode

ReadMode is used by the [Read](#read) function. It defines what to do with the 
file if it exists or not

```c
typedef enum _ReadMode {
    ATRC_READ_ONLY,
    ATRC_CREATE_READ,
    ATRC_FORCE_READ,
} ReadMode;
```

#### ATRC_READ_ONLY
Reads file
#### ATRC_CREATE_READ
Creates the file if it doesn't exist and reads it. Fileheader is added automatically
#### ATRC_FORCE_READ
Deletes the file if it exists, creates a new one and reads it. Fileheader is added automatically

---

## Data Structures

### C_Variable

Data structure for a variable. Freeing and allocating is done by the library.

```c
typedef struct _C_Variable {
    char *Name;
    char *Value;
    bool IsPublic;
    uint64_t line_number;
} C_Variable, *C_PVariable;
```

#### Name
Name of the variable
#### Value
Value of the the variable
#### IsPublic
Is the variable public. See more about private and public variables in `syntax.md`-`Variables`

#### line_number
Line number of the variable

### C_Variable_Arr

Data structure for an array of variables. Freeing and allocating is done by the library

```c
typedef struct _C_Variable_Arr {
    C_Variable *Variables;
    uint64_t VariableCount;
} C_Variable_Arr, *C_PVariable_Arr;
```

#### Variables
Pointer to the list of variables
#### VariableCount
Amount of variables

### C_Key

Data structure for a key. Freeing and allocating is done by the library

```c
typedef struct _C_Key {
    char *Name;
    char *Value;
    uint64_t line_number;
    uint64_t enum_value;
} C_Key, *C_PKey;
```

#### Name
Name of the key
#### Value
Value of the key
#### line_number
Line number of the key
#### enum_value
Enum value of the key. For more information, see `advanced_features.md`-`Enums`

### C_Block

Data structure for a block. Freeing and allocating is done by the library

```c
typedef struct _C_Block {
    char *Name;
    C_Key *Keys;
    uint64_t KeyCount;
    uint64_t line_number;
} C_Block, *C_PBlock;
```

#### Name
Name of the key
#### Keys
Pointer to an array of keys
#### KeyCount
Amount of keys inside the block
#### line_number
Line number of the block

### C_Block_Arr

Data structure for an array of blocks. Freeing and allocating is done by the library

```c
typedef struct _C_Block_Arr {
    C_Block *Blocks;
    uint64_t BlockCount;
} C_Block_Arr, *C_PBlock_Arr;
```

#### Blocks
Pointer to the blocks
#### BlockCount
Amount of blocks

### C_ATRC_FD

File data structure. Free with [Destroy_ATRC_FD](#destroy_atrc_fd).

```c
typedef struct _ATRCFiledata{
    C_PVariable_Arr Variables;
    C_PBlock_Arr Blocks;
    char *Filename;
    bool AutoSave;
    bool Writecheck;
} C_ATRC_FD, *PATRC_FD;
```

#### Variables
    Array of variables
#### Blocks
    Array of blocks
#### Filename
    Filename
#### AutoSave
Structures autosave status. 
  When AutoSave is set to true, all changes to the variables, keys 
and blocks are saved into the file.
  When AutoSave is false, changes will not be saved into the file.
  False by default
  
#### Writecheck
Structures writecheck status.
  If writecheck is true and user is modifying a variable, it will create a new one if one doesn't exist while modifying it.
  If writecheck is true and user is modifying a key, it will create a new block and key if one doesn't exist while modifying it. 
---

## File Operations

### Read

Reads an ATRC configuration file, returning TRUE on success, FALSE otherwise. Before reading one, you need to create the structure. See [Constructor/Destructor Functions](#constructordestructor-functions)

```c
ATRC_API bool Read(PATRC_FD self, const char* path, ReadMode readMode);
```

#### self
Pointer to C_ATRC_FD
#### path
File path
#### readMode
Read mode. See [ReadMode](#readmode)
#### Returns
TRUE on success, FALSE otherwise

---

## Data Access Functions

### ReadVariable

Reads a variable

```c
ATRC_API const char* ReadVariable(PATRC_FD self, const char* varname);
```

#### self
Pointer to the C_ATRC_FD structure
#### varname
Variable's name to read
#### Returns
Variable's value. NULL on error


### ReadKey

Reads a key from a block

```c
ATRC_API const char* ReadKey(PATRC_FD self, const char* block, const char* key);
```

#### self
Pointer to the C_ATRC_FD structure
#### block
Name of the block
#### key
Name of the key
#### Returns
Key's value. NULL on error

---

## Existence Check Functions

### DoesExistBlock

Checks if a Block exists.

```c
ATRC_API bool DoesExistBlock(PATRC_FD self, const char* block);
```

#### self
Pointer to the C_ATRC_FD structure
#### block
name of the block
#### Returns
TRUE if block exists, false otherwise

### DoesExistVariable

Checks if a Variable exists.

```c
ATRC_API bool DoesExistVariable(PATRC_FD self, const char* varname);
```
#### self
Pointer to the C_ATRC_FD structure
#### varname
Variable's name
#### Returns
TRUE if exists, FALSE otherwise

### DoesExistKey

Checks if a Key exists.

```c
ATRC_API bool DoesExistKey(PATRC_FD self, const char* block, const char* key);
```
#### self
Pointer to the C_ATRC_FD structure
#### block
Block's name
#### key
Key's name
#### Returns
TRUE if exists, FALSE otherwise

### IsPublic

Checks if a variable is public or private. 
See more about private and public variables in `syntax.md`-`Variables`

```c
ATRC_API bool IsPublic(PATRC_FD self, const char* varname);
```

#### self
Pointer to the C_ATRC_FD structure
#### varname
Variable's name
#### Returns
TRUE if variable is public, FALSE otherwise

---

## Variable Injection

### InsertVar_S

Inserts values into inject markings. 
See `syntax.md`-`Insert/Inject Markings` for more information
For easier usage, see the macro `ATRC_CINJECT` in `macros_constants.md`

```c
ATRC_API char* InsertVar_S(const char* line, const char** args);
```

#### line
The line containing inject markings
#### args
A null terminated list of strings
#### Returns
Injected line. Free results with [ATRC_FREE_MEMORY](#atrc_free_memory)

#### Example
```c
const char *message = "String containing %*% markings";
const char *args[] = {"inject", NULL};
res = InsertVar_S(message, args);
printf("%s\n", res); // -> String containing inject markings
ATRC_FREE_MEMORY(res);
```

---

## Block Management

### AddBlock

Creates a new block. 
If AutoSave is turned on, the block will be added to the top of the file, after fileheader, comments and variable definitions.

```c
ATRC_API bool AddBlock(PATRC_FD self, const char* blockname);
```

#### self
Pointer to the C_ATRC_FD structure
#### blockname
New block to create
#### Returns
TRUE on success, FALSE otherwise

### RemoveBlock

Removes a block. 
If autosave is turned on, everything below the blocks definition is deleted until a second block definition is found. This includes comments and variables

```c
ATRC_API bool RemoveBlock(PATRC_FD self, const char* blockname);
```

#### self
Pointer to the C_ATRC_FD structure
#### blockname
Block to delete
#### Returns
TRUE on success, FALSE otherwise

---

## Variable Management

### AddVariable

Creates a new variable.
If autosave is on, the new variable will be added after the fileheader and comments.

```c
ATRC_API bool AddVariable(PATRC_FD self, const char* varname, const char* value);
```

#### self
Pointer to the C_ATRC_FD structure
#### varname
Variable to create
#### Returns
TRUE on success, FALSE otherwise

### RemoveVariable

Deletes a variable

```c
ATRC_API bool RemoveVariable(PATRC_FD self, const char* varname);
```

#### self
Pointer to the C_ATRC_FD structure
#### varname
Variable to delete
#### Returns
TRUE on success, FALSE otherwise

### ModifyVariable

Modifies a variable's value with a given new one

```c
ATRC_API bool ModifyVariable(PATRC_FD self, const char* varname, const char* value);
```

#### self
Pointer to the C_ATRC_FD structure
#### varname
Variable's name
#### value
New value
#### Returns
TRUE on success, FALSE otherwise

---

## Key Management

### AddKey

Adds a new key into a block.
If autosave is turned on, the key will be added right after the block definition.

```c
ATRC_API bool AddKey(PATRC_FD self, const char* block, const char* key, const char* value);
```

#### self
Pointer to the C_ATRC_FD structure
#### block
Block's name
#### key
Key's name
#### Returns
TRUE on success, FALSE otherwise


### RemoveKey

Remove's a key from the given block

```c
ATRC_API bool RemoveKey(PATRC_FD self, const char* block, const char* key);
```

#### self
Pointer to the C_ATRC_FD structure
#### block
Block's name
#### key
Key's name
#### Returns
TRUE on success, FALSE otherwise


### ModifyKey

Modifies a key from a block with the new given value

```c
ATRC_API bool ModifyKey(PATRC_FD self, const char* block, const char* key, const char* value);
```

#### self
Pointer to the C_ATRC_FD structure
#### block
Block's name
#### key
Key's name
#### value
New value
#### Returns
TRUE on success, FALSE otherwise


---

## Comment Functions

### WriteCommentToBottom

Writes a comment on the last row of the file

```c
ATRC_API bool WriteCommentToBottom(PATRC_FD self, const char* comment);
```

#### self
Pointer to the C_ATRC_FD structure
#### comment
Comment to write
#### Returns
TRUE on success, FALSE otherwise

#### Remarks
If the comment written is "This is a comment", the comment will look like this inside the file: "#This is a comment". 

### WriteCommentToTop

Writes a comment to the top of the file, just below the fileheader

```c
ATRC_API bool WriteCommentToTop(PATRC_FD self, const char* comment);
```

#### self
Pointer to the C_ATRC_FD structure
#### comment
Comment to write
#### Returns
TRUE on success, FALSE otherwise

#### Remarks
If the comment written is "This is a comment", the comment will look like this inside the file: "#This is a comment". 

---

## Enum Functions

### GetEnumValue

Gets the enum value of a key from a block. 
For more information, see `advanced_features.md`-`Enums`

```c
ATRC_API uint64_t GetEnumValue(PATRC_FD self, const char* block, const char* key);
```

#### self
Pointer to the C_ATRC_FD structure
#### block
Block's name
#### key
Key's name
#### Returns
Enum value of the key. (uint64_t)-1 on errors

---

## Constructor/Destructor Functions

### Create_ATRC_FD

Creates a new [C_ATRC_FD](#c_atrc_fd) structure, with data from the given filename. Free with [Destroy_ATRC_FD](#destroy_atrc_fd)

```c
ATRC_API PATRC_FD Create_ATRC_FD(char *filename, ReadMode readMode);
```

#### filename
Path to the atrc file
#### readMode
File read mode, see [ReadMode](#readmode)
#### Returns
Pointer to a new [C_ATRC_FD](#c_atrc_fd) structure

#### Remarks
Free with [Destroy_ATRC_FD](#destroy_atrc_fd)

### Create_Empty_ATRC_FD

Creates an empty [C_ATRC_FD](#c_atrc_fd) structure. Free with [Destroy_ATRC_FD](#destroy_atrc_fd)

```c
ATRC_API PATRC_FD Create_Empty_ATRC_FD();
```

#### Returns
Pointer to a new, empty [C_ATRC_FD](#c_atrc_fd) structure

#### Remarks
Free with [Destroy_ATRC_FD](#destroy_atrc_fd).
Once read, use [Read](#read) to read data into the structure


### Destroy_ATRC_FD_Blocks_And_Keys

Frees memory allocated by blocks and keys from a C_ATRC_FD structure

```c
ATRC_API void *Destroy_ATRC_FD_Blocks_And_Keys(PATRC_FD self);
```

#### self
Pointer to C_ATRC_FD
#### Returns
Pointer to `self`

### Destroy_ATRC_FD_Variables

Frees memory allocated by variables

```c
ATRC_API void *Destroy_ATRC_FD_Variables(PATRC_FD self);
```

#### self
Pointer to C_ATRC_FD
#### Returns
Pointer to `self`

### Destroy_ATRC_FD

Frees all the memory allocated my a C_ATRC_FD

```c
ATRC_API void *Destroy_ATRC_FD(PATRC_FD self);
```

#### self
Pointer to C_ATRC_FD
#### Returns 
Pointer to `self`. NULL on success, anything else otherwise

---

## Memory Management

### ATRC_FREE_MEMORY

Frees memory allocated by ATRC. Never use free() or delete[] with ATRC if not told otherwise!

```c
ATRC_API void *ATRC_FREE_MEMORY(void *ptr);
```

#### ptr
Memory to free
#### Returns
Pointer to `ptr`. NULL on success, anything else otherwise