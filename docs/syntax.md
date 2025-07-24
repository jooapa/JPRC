# ATRC Syntax

## Table of Contents

- [Reserved Characters](#Reserved-Characters-and-Their-Allowed-Usage)
- [Insert/Inject Markings](#insertinject-markings)
- [Header](#header)
- [Variables](#variables)
- [Blocks](#blocks)
- [Keys](#keys)
- [Comments](#comments)

## Reserved Characters and Their Allowed Usage

No block, variable or key name can contain:

### Reserved Characters

- `[`
- `]`
- `#`
- `*`
- `%`
- `&`

## Allowed Usage 

- #
    - Denotes a comment.
    - To use inside a value, use backslash: `\#` 
- %
    - Denotes variable or injection
    - To use in a value, use backslash: `%`
- &
    - Denotes whitespace
    - To use in a value, use backslash

### Example

```ini
#!ATRC
[BlockName]
TestKey=&This value uses reserved&\&important characters!
# Outputs: " This value uses reserved & important characters!
```

## Insert/Inject Markings

Inject markings are denoted in two of the following ways

- %*%
    - Values are injected from left to right, in the order these markings appear.
- %*<num>%
    - Values are inject in the numeral order they are tagged as.
    - First value is accessed with 0.
    - Maximum <num> allowed is `9999`
### Example

```ini
#!ATRC
# Inject array: { "Hello", ",", "World!" }
%example_1%=%*%%*% %*% # This outputs "Hello, World!"
%example_2%%*2*%%*0% %*1% # This outputs "World!Hello ,"
```


## Header
First row in the resource file should be `#!ATRC`, thus denoting the file as an ATRC file and the extension can be freely choosed.

## Variables
## Blocks
## Keys
## Comments
There are two different ways of making comments/remarks in a file. 
First one is with the comment character: `#`. 
Another one is with a preprocessor directive `.IGNORE <value>`. See preprocessor.md for more information

### Example

```ini
#!ATRC
# This line is a comment

#.IGNORE 4
These following
4 lines
are ignored
by the parser
```


## Preprocessor directives

### Conditional Logic
### File Inclusion
### Platform Detection
### Raw Strings
### Miscellaneous

## Advanced Features

### Variable Injection
### Enums