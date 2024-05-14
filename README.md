# ATRC resource file library made with C#

Uses dotnet 8.0, works on linux, windows and propably macos

--- 

Build nupkg: ```.\ATRC\BuildNupkg.bat```

Run test project ```.\ATRC.TEST\Run.bat```

### Usage in c#

```csharp
using ATRC;
namespace Test
{
    public class Program
    {

        private static ATRC.Filehandler.ATRCFileData fileData = new(); // This will contain the file data
        
        static void Main(string[] args)
        {
            fileData.Read("file.atrc"); // Read and parse file.atrc
            fileData.AddBlock("test"); // Create new block
            fileData.AddKey("test", "test_key", "my_value"); // Create new key to test-block with my_value as its value
            string[] test_array = {"my", "values"};
            fileData.AddKey("test", "array_key", test_array); // Create new array key

            // Read both keys
            string contents_1 = fileData.S_ReadKey("test", "test_key"); // Read string key
            string[] contents_2 = fileData.A_ReadKey("test", "array_key"); // Read array key
    
            // Create new variable if it does not exist
            if(!fileData.VariableExists("system32_path")){
                fileData.AddVariable("system32_path", "C:\\Windows\\System32");
            }

            // Remove array and test_key
            fileData.RemoveKey("test", "array_key");
            fileData.ModifyKey("test", "test_key", "%system32_path%\\explorer.exe");

            // Move key to other location
            fileData.AddBlock("WinShell");
            fileData.MoveKey("test", "WinShell", "test_key");

            Console.WriteLine(fileData.S_ReadKey("WinApps", "test_key")); // -> C:\Windows\System32\notepad.exe
            /*
            file.atrc ->
            %system32_path%=C:\Windows\System32
            [WinApps]
            test_key=%system32_path%\notepad.exe
            */
        }
    }
}
```

### Usage in .atrc
```
! whitespaces are kept after = sign, if you want to add whitespace use & and everything is case-sensitive
! create a new block with []
! variable, key or block names can't contain: !, %, & or ,
! variables are constants
%variable%=value
<%private_variable%=private_value!This can't be accessed be modified or read outside the file

[blockname]
key1=value
variable_list=%variable%, %private_variable%! when reading this, csharp will show {"value", " private_value"}

whitespace = hello 
whitespace2 = hello&
! Key would be 'whitespace' and its key ' hello' 
! If you want to include !, %, &, or , in your value, use \
reserved_characters=this\, is \&\% reserved\!
```

### ATRCFileData methods:

 - AddBlock         -> Adds given block
 
 - RemoveBlock      -> Removes given block

 - BlockExists      -> Check if block exists

 - AddVariable      -> Adds given variable with its value

 - A_ReadVariable   -> Reads list variable, returning contents in a string[]

 - S_ReadVariable   -> Reads variable, returning contents in a string

 - ModifyVariable   -> Modify given variable with given value

 - RemoveVariable   -> Removes given variable

 - VariableExists   -> Checks whether variable exists

 - IsPrivateVariable-> Checks if variable is private

 - AddKey           -> Adds key to the given block

 - A_ReadKey        -> Reads list key, returning its contents in a string[]
 
 - S_ReadKey        -> Reads key, returning its contents in a string

 - ModifyKey        -> Modifies given key with the given value

 - RemoveKey        -> Remove the given key from given block

 - MoveKey          -> Move key from source to destination

 - KeyExists        -> Checks whether the key exists