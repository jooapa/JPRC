# ATRC resource file library made with C#

## Uses dotnet 8.0

```
! If used in strings or chars, the following characters need to use backslash '\' before them for the program to be able to read them: ! , = " [ ]
! ! - Single line comment
! , - Tuple and vector value divider
! = - Assign value to key
! 
! If you want your numbers to be interpeted as strings, wrap them with ""
!
! Key types: String, char, integer, float, array, tuple 
!
! Wrap block names in square brackets
[Strings]

!Char
Char1=a

!String
String1=thisisa\"string\"

[Arrays]
!Array
Array1=hello,this,is,vector,wi\,th,comma

!Tuple
Tuple1=this,is,1,tuple\!

```

