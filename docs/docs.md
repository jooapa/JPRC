# .ATRC

## syntax
- Variable:
    - `%var_name%=value`
- Block:
    - `[block_name]`
- Key:
    - `key_name=value`
- Insert markings: 
    - `%*%`
    - `%*0%`, `%*1%`, `%*2%`...
- Comment marking:
    - `!`
- Whitespace marking:
    - `&`

# .CPP

## ATRC Structs

### Variable

```cpp
struct Variable {
    std::string Name;
    std::string Value;
    bool IsPublic;
};
```

#### fields
 - `Name`: Variable name
 - `Value`: Variable value
 - `IsPublic`: Is variable defined as public

#### remarks
Private variables are saved but can't be read with ATRC Functions

### Block
```cpp
struct Block {
    std::string Name;
    std::vector<Key> Keys;
};
```

#### fields
 - `Name`: Block name
 - `Keys`: Array of keys inside the block



### Key
```cpp
struct Key {
    std::string Name;
    std::string Value;
};
```

#### fields
 - `Name`: Key name
 - `Value`: Key value



### _ATRCFiledata
```cpp
typedef struct _ATRCFiledata{
    std::unique_ptr<std::vector<Variable>> Variables;
    std::unique_ptr<std::vector<Block>> Blocks;
    std::string Filename;
    std::string Extension;
    std::string Encoding;

    bool AutoSave = false;
} ATRC_FD, *PATRC_FD;
```

#### fields
 - `Variables`: All the variables inside ATRC file
 - `Blocks`: All the blocks inside ATRC file
 - `Filename`: Filename. See Read() for more information.
 - `Extension`: File extension. See Read() for more information.
 - `Encoding`: File encoding. See Read() for more information.
 - `AutoSave`: AutoSave saves the changes that happen while AutoSave is true

#### remarks
 The only value you should change yourself is `AutoSave`

## ATRC Functions

### Read

Reads atrc file

```cpp
ATRC_API std::shared_ptr<ATRC_FD> 
Read
(
    const std::string& filename = "", 
    const std::string& encoding = "utf-8", 
    const std::string& allowed_extension = ""
);
```

#### args
 - `filename`: Path to the resource file
 - `encoding`: File encoding. Only values avaiable: "utf-8"
 - `allowed_extension`: Allows parser to read other than ".atrc" files

#### returns
 - `std::shared_ptr<ATRC_FD>`: Pointer to ATRC FileData.

#### example(s)

```cpp
std::string filename = "test.atrc";
std::shared_ptr<ATRC_FD> fd = Read(filename);
```
```cpp
// For other extensions with ATRC syntax:
std::string filename = "test.ini";
std::shared_ptr<ATRC_FD> fd = Read(filename, "utf-8", ".ini");
```



### ReadVariable

Reads variable's value

```cpp
ATRC_API std::string 
ReadVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname
);

```

#### args
 - `filedata`: filedata's shared ptr
 - `varname`: Variable to read

#### returns
 - Variable's value
 - If variable is private or it doesn't exist, empty string ("") is returned

#### remarks
 ReadVariable doesn't check if variable exists.

#### example(s)

```cpp
if(DoesExistVariable(fd, "varname")) {
    std::string res = ReadVariable(fd, "varname");
    std::cout << res << std::endl;
}
```



### ReadKey

Reads key's value

```cpp
ATRC_API std::string 
ReadKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `block`: Block name
 - `key`: Key name

#### returns
 - Key's value

#### remarks
 ReadKey doesn't check if the key or block exists

#### example(s)

```cpp
if(DoesExistBlock(fd, "block") && DoesExistKey(fd, "block", "key")) {
    std::cout << ReadKey(fd, "block", "key") << std::endl;
}
```



### DoesExistBlock

Checks if block exists

```cpp
ATRC_API bool 
DoesExistBlock
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `block`: Block name

#### returns
 - True if block exists, false if not

#### example(s)

```cpp
if(DoesExistBlock(filedata, "block")) std::cout << "Block exists!\n";
```



### DoesExistVariable

Checks if variable exists

```cpp
ATRC_API bool 
DoesExistVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `varname`: Variable name

#### returns
 - True if variable exists, false otherwise

#### remarks
 False is also returned if variable is marked as private

#### example(s)
```cpp
if(DoesExistVariable(filedata, "varname")) std::cout << "Variable exists!\n";
```



### DoesExistKey

Checks if key exists

```cpp
ATRC_API bool 
DoesExistKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key
);
```


#### args
 - `filedata`: filedata's shared ptr
 - `block`: Block name
 - `key`: Key name

#### returns
 - True if key exists, false otherwise

#### remarks
 DoesExistKey doesn't check if block exists

#### example(s)

```cpp
if(DoesExistKey(filedata, "key")) std::cout << "Key exists!\n";
```



### IsPublic

Checks if variable is public

```cpp
ATRC_API bool 
IsPublic
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `varname`: Variable name

#### returns
 - Returns true if variable is public, false otherwise

#### remarks
 IsPublic does not check if variable exists

#### example(s)

```cpp
if(IsPublic(fd, "varname")){
    std::cout << ReadVariable(fd, "varname") << std::endl;
}
```



### InsertVar

Inserts/Injects text to insert marks

```cpp
ATRC_API void 
InsertVar
(
    std::string &line, 
    std::vector<std::string> &args, 
    std::shared_ptr<ATRC_FD> filedata
);
```

#### args
 - `line`: String with insert markings. Result is saved here aswell
 - `args`: Array with text to insert
 - `filedata`: filedata's shared ptr

#### returns
 - void. Result is saved to line
 
#### remarks
 If the program runs out of insert markings or args, error message is being displayed.

#### example(s)

```cpp
std::vector<std::string> inserts = { "1.", "2.", "3." };
std::string value = ReadKey(fd, "block", "varname");
InsertVar(value, inserts, fd);
std::cout << value << std::endl;
```



### AddBlock

Adds block

```cpp
ATRC_API void 
AddBlock
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& blockname
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `blockname`: Block name to  add

#### remarks
 If AutoSave is on, the block is added to the end of the file.
 Functions checks if similar block already exists.

#### example(s)

```cpp
AddBlock(fd, "test");
```



### RemoveBlock

Removes block

```cpp
ATRC_API void 
RemoveBlock
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& blockname
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `blockname`: Block to remove

#### remarks
 RemoveBlock checks if block exists before removing it.
 If autosave is on, contents from the start of the block until the next block or the end of the file are remove.

#### example(s)

```cpp
RemoveBlock(fd, "block");
```



### AddVariable

Adds variable

```cpp
ATRC_API void 
AddVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname, 
    const std::string& value
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `varname`: name for new variable
 - `value`: value for new variable

#### remarks
 Checks if variable exists before creating a new one
 If AutoSave is on, new variable is added to the start of the file

#### example(s)

```cpp
AddVariable(fd, "var_name", "value");
```



### RemoveVariable

```cpp
ATRC_API void 
RemoveVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `varname`: variable to delete

#### remarks
 Checks if varname exists before removing

#### example(s)

```cpp
RemoveVariable(fd, "varname");
```



### ModifyVariable

Updates variable's value to a new value

```cpp
ATRC_API void 
ModifyVariable
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& varname, 
    const std::string& value
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `varname`: variable name to modify
 - `value`: new value

#### remarks
 Checks if variable exists before modifying it

#### example(s)

```cpp
ModifyVariable(fd, "varname", "new_value");
```



### AddKey

Adds new key to a given block

```cpp
ATRC_API void 
AddKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key, 
    const std::string& value
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `block`: Block where key will be added to
 - `key`: Key name
 - `value`: Key's value

#### remarks
 Checks if block and key exist before creating the new one

#### example(s)

```cpp
AddKey(fd, "block_name", "key_name", "Value");
```



### RemoveKey

Removes key from a block

```cpp
ATRC_API void 
RemoveKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `block`: Block where the key is located
 - `key`: Key's name

#### remarks
 Checks if block and key exist before removing it

#### example(s)

```cpp
RemoveKey(fd, "block", "key");
```



### ModifyKey

Modifies key's value

```cpp
ATRC_API void 
ModifyKey
(
    std::shared_ptr<ATRC_FD> filedata, 
    const std::string& block, 
    const std::string& key, 
    const std::string& value
);
```

#### args
 - `filedata`: filedata's shared ptr
 - `block`: Block where the key is located
 - `key`: Key's name
 - `value`: Key's new value

#### remarks
 Checks if block and key exist before modifying it

#### example(s)

```cpp
ModifyKey(fd, "blockname", "keyname", "newvalue");
```



## ATRC STDLIB

*Include by defining `INCLUDE_ATRC_STDLIB` before header import*

### atrc_stdlib_errval

```cpp
#define     SUCCESSFULL_ACTION      1
#define     UNSUCCESFULL_ACTION     0
ATRC_API extern int atrc_stdlib_errval;
```

#### remarks

every stdlib functions sets `atrc_stdlib_errval`

 - `SUCCESSFULL_ACTION`: Defined action was done succesfully
 - `UNSUCCESFULL_ACTION`: Defined action was not done succesfully



### atrc_to_list

Turns a string value to std::vector, seperating it by a given character

```cpp
std::vector<std::string> 
atrc_to_list
(
    char separator, 
    const std::string &value
)
```

#### args

 - `separator`: Separator character for value
 - `value`: Value to parse

#### returns

Returns a `std::vector<std::string>`

Sets `SUCCESSFULL_ACTION` when list is done

#### remarks

Value doesn't have to end with separator character

#### example(s)

```cpp
// Key value is: this;is;a;test;
std::vector<std::string> res = atrc_to_list(';', ReadKey(fd, "block", "key"));
for(std::string &s : res){
    std::cout << s << "\n";
}
```


### atrc_to_bool

Casts string to bool

```cpp
bool 
atrc_to_bool
(
    const std::string &value
)
```

#### args

 - `value`: Value to parse

#### returns

 - True if value is either "TRUE" or "1"
 - False if value is either "FALSE" or "0"

 - On failure, false is returned

 - Sets `SUCCESSFULL_ACTION` when value is parsed succesfully
 - Sets `UNSUCCESFULL_ACTION` when value is not "TRUE", "FALSE", "0" or "1"

#### remarks

Value is not case-sensitive. True, TrUe, TRUE and true all work

#### example(s)

```cpp
bool bool_res = atrc_to_bool(ReadKey(fd, "block", "key"));
if(atrc_stdlib_errval == SUCCESSFULL_ACTION) 
    std::cout << bool_res << "\n";
```



### atrc_to_uint64_t

Casts string to unsigned integer

```cpp
ATRC_API uint64_t 
atrc_to_uint64_t
(
    const std::string &value
);
```

#### args
 - `value`: String value to cast

#### returns
 - `uint64_t`: Converted string value
 - `SUCCESSFULL_ACTION`: If cast was succesfull
 - `UNSUCCESSFULL_ACTION`: If cast was unsuccesfull

#### example(s)

```cpp
uint64_t res = atrc_to_uint64_t(ReadVariable(fd, "var"));
```



### atrc_to_int64_t

Casts value to signed integer

```cpp
ATRC_API int64_t 
atrc_to_int64_t
(
    const std::string &value
);
```

#### args
 - `value`: String value to cast

#### returns
 - `int64_t`: Converted string value
 - `SUCCESSFULL_ACTION`: If cast was succesfull
 - `UNSUCCESSFULL_ACTION`: If cast was unsuccesfull

#### example(s)

```cpp
int64_t res = atrc_to_int64_t(ReadVariable(fd, "var"));
```



### atrc_to_double

Casts string value to double

```cpp
ATRC_API double 
atrc_to_double
(
    const std::string &value
);
```

#### args
 - `value`: String value to cast

#### returns
 - `double`: Converted string value
 - `SUCCESSFULL_ACTION`: If cast was succesfull
 - `UNSUCCESSFULL_ACTION`: If cast was unsuccesfull

#### example(s)

```cpp
double res = atrc_to_double(ReadVariable(fd, "var"));
```



## ATRC Error messages

 - `ATRC ERROR<{CLASS/FILE}?{TYPE}><{filename|filepath}>: {ERROR MESSAGE}`
 - `ATRC Error<300?304><test.atrc>: Block already exists: 'blockname'`
 - `ATRC Error<300?305><test.atrc>: Key already exists: 'keyname'`
 - `ATRC Error<100?107><test.atrc>: Re-Referenced key: 'keyname' at line 6`

### Error numbers
 ```cpp
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
 ```
