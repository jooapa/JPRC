# ATRC Standard Library

ATRC Standard library (stdlib, atrc_stdlib) contains helpful functions 
for fiddling with the values found inside ATRC files. Can be used with normal strings aswell

## ATRC_ERR

Values used by the `atrc_stdlib_errval`

```c
enum ATRC_ERR {
	_ATRC_SUCCESSFULL_ACTION = 0,
	_ATRC_UNSUCCESSFULL_ACTION = 1
};
```

- _ATRC_SUCCESSFULL_ACTION 
    - Action was succesfully done
- _ATRC_UNSUCCESSFULL_ACTION
    - An error occured during the action

## atrc_stdlib_errval

Stdlib functions change this variable according if the function does what it was intended to do

- _ATRC_SUCCESSFULL_ACTION 
    - Action was succesfully done
- _ATRC_UNSUCCESSFULL_ACTION
    - An error occured during the action

```c
extern uint64_t atrc_stdlib_errval;
```

## _C_String_Arr

Array created by `atrc_to_list`. Free with `atrc_free_list`

```c
typedef struct _C_String_Arr {
    char **list;
    uint64_t count;
} C_String_Arr, *PString_Arr;
```

## atrc_to_list

Same as `atrc_to_vector` but works with C. 
Free with `atrc_free_list

```c
ATRC_API PString_Arr atrc_to_list(const char separator, const char* value);
```

- `const char separator`
    - Characted which to separate values by
- `const char* value`
    - Value, which to turn into the list

### Example
```c
PString_Arr res = atrc_to_list('|', "this|is|a|list");
for(int i = 0; i < res.count; i++) {
    printf(res.list[i]);
}
atrc_free_list(res);
```

## atrc_free_list

Frees `PString_Arr`, returning NULL on success

```c
ATRC_API void *atrc_free_list(PString_Arr list);
```

- `PString_Arr list`
    - Pointer to list which to free

## atrc_to_bool

Turns string into a boolean value. 
String is case-insensitive.

Following values will return true:
    - TRUE, 1, YES, ON

Following values will return false:
    - FALSE, 0, NO, OFF

```c
ATRC_API bool atrc_to_bool(const char* value);
```

## atrc_to_uint64_t

Turns value into unsigned 64-bit integer

```c
ATRC_API uint64_t atrc_to_uint64_t(const char* value);
```

## atrc_to_int64_t

Turns value into signed 64-bit integer

```c
ATRC_API int64_t atrc_to_int64_t(const char* value);
```

## atrc_to_double

Turns value into a double

```c
ATRC_API double atrc_to_double(const char* value);
```

## atrc_math_exp

Parses a math expression

Expression string can hold the following values:
 - Numbers, floats. (decimals: 0, 0.0) (hexadecimals: 0x0, 0X0) (binary: 0b00000000,0B00000000)
 - PI
 - E
 - GR (Golden Ratio)
 - SQRT2
 - LOG2E
 - LOG10E
 - +, -, *, /, %, (, )
 - ^{val} (Power to)
 - SQRT({val})
 - ABS({val})
 - LOG({val})
 - LOG10({val})
 - SIN({val})
 - COS({val})
 - TAN({val})
 - ASIN({val})
 - ACOS({val})
 - ATAN({val})

```c
ATRC_API double atrc_math_exp(const char* value);
```

### Example

```cpp
const char *expression = "PI * 10 ^2";
double area = atrc_math_exp(expression);
std::cout << area << std::endl;
```
