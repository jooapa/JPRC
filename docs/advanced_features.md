# Advanced features

- [Variable Injection](#variable-injection)
- [Enums](#enums)

## Variable Injection

Inject markings are denoted in two of the following ways

- %*%
    - Values are injected from left to right, in the order these markings appear.
- %*<num>%
    - Values are inject in the numeral order they are tagged as.
    - First value is accessed with 0.
    - Maximum <num> allowed is `9999`

Values can be injected into the markings with `InsertVar` or `InserVar_S`. 
See c_api.md or cpp_api.md for more information. 

Values can be injected into the markings with `ATRC_INJECT` or `ATRC_CINJECT`
See macros_constants.md for more information

### Example

```ini
#!ATRC
# Inject array: { "Hello", ",", "World!" }
%example_1%=%*%%*% %*% # This outputs "Hello, World!"
%example_2%%*2*%%*0% %*1% # This outputs "World!Hello ,"
```

```cpp
#include <ATRC.h>
#using namespace atrc;

int main(){
    ATRC_FD fd = ATRC_FD("./test.atrc", ReadMode::ATRC_READ_ONLY);
    if (!fd.CheckStatus()) return 1;
    std::vector<std::string> injects = { "Hello", ",", "World!"}
    std::string res = fd["example_2"];
    fd.InsertVar(res, injects);
    std::cout << res << std::endl;
    return 0;
}
```

## Enums

Enum values are automatically assigned to keys inside a block and can 
be accessed GetEnumValue. See c_api.md or cpp_api.md for more information.

### Example

```ini
#!ATRC
[EnumTest]
Key1=Value
Key2=Value
Key3=Value

# Read EnumTest with GetEnumValue. See c_api.md or cpp_api.md for more information
# Key1 = 0
# Key2 = 1
# Key3 = 3
```

```cpp
#include <ATRC.h>
#using namespace atrc;

int main(){
    ATRC_FD fd = ATRC_FD("./test.atrc", ReadMode::ATRC_READ_ONLY);
    if (!fd.CheckStatus()) return 1;
    uint64_t enum_val = fd.GetEnumValue(fd["EnumTest"]["Key1"]);
    std::cout << enum_val << std::endl;
    return 0;
}
```