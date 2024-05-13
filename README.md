# ATRC resource file library made with C#

Uses dotnet 8.0, works on linux, windows and propably macos

--- 

Build nupkg: ```.\ATRC\BuildNupkg.bat```

Run test project ```.\ATRC.TEST\Run.bat```


| Function | arg1 | arg2 | arg3 | usage | will make exception if |
|      ----|  ----|  ----|  ----|  ----|         ----|
|void Read(string p_filename)|path to file|| | read .atrc file| filename isn't .atrc, invalid p_mode, read and file does not exist |
|void AddBlock(string block)|block name to be created|||create a new empty block|block contains reserved words (see Usage in .atrc)|
|public void RemoveBlock(string block)|block name to be removed|||removes block along its contents|block does not exist|
|void AddVariable(string name, object value)|variable's name to be added|variable's value, string or string[]||create a new list or string variable|name contains reserved words (see Usage in .atrc), variable already exists or it's not string or string[]|
|string[] A_ReadVariable(string name)|variable to be read|||returns list variables value|variable doesn't exist or variable is private|
|string S_ReadVariable(string name)|variable to be read|||returns string variables value|variable doesn't exist or variable is private|
|void ModifyVariable(string name, object value)|variable to be modified|string or string[] value||modify value of variable|value is not string or string[], variable doesn't exist or variable is private|
|void RemoveVariable(string name)|variable to be removed|||remove variable|variable doesn't exist or variable is private|
|void AddKey(string block, string key, object value)|block where to add|key name to be added|its value, string or string[]|add a key to a block|value is not string or string[], block or key contains reserved keywords (see Usage in .atrc), block does not exist or key already exists in destination|
|string[] A_ReadKey(string block, string key)|block where to read|key to read||read list key from block, returning it as string[]|key or block does not exist|
|string S_ReadKey(string block, string key)|block where to read|key to read|  |read string key from block, returning it as string|key or block does not exist|
|void ModifyKey(string block, string key, object value)|block where it's located|key to modify|new value, string or string[]|modify existing key's value|block or key doesn't exist, value isn't string or string[]|
|void RemoveKey(string block, string key)|key location|key to be removed||remove key from block|block or key does not exist|
|void MoveKey(string fromBlock, string toBlock, string key)|move from source|to destination|key to be moved|move key from src to dst|neither block exist, key exists in dst or key doesn't exist in src|
|bool ParseToBool(string value)|value to be parsed|||parses string to boolean|if value is not boolean|
|int ParseToInt(string value)|value to be parsed|||parses string to integer|if value is not integer|
|float ParseToFloat(string value)|value to be parsed|||parses string to float|if value is not float|

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
            
            // Read file 'test.atrc'
            fileData.Read("file.atrc", "r");
            
            // Creata new blocks 'test' and 'test2'
            fileData.AddBlock("test");
            fileData.AddBlock("test2");
            
            // Create two new keys 'key1' and 'key2' in block 'test' with values 'value1' and 'value2'
            fileData.AddKey("test", "key1", "value1");
            fileData.AddKey("test", "key2", "value2");

            string value = fileData.S_ReadKey("test", "key1"); // Read key 'key1' in block 'test'
            Console.WriteLine(value); // Output: value1

            // Remove key2 and move key1 to block 'test2'
            fileData.RemoveKey("test", "key2");
            fileData.MoveKey("test", "test2", "key1");

            // We can save arrays as well
            string[] array = new string[] { "value1", "value2", "value3" };
            fileData.ModifyKey("test2", "key1", array); // Save array to 'key1'

            // Remove the empty block 'test'
            fileData.RemoveBlock("test");
            

            // Create a new variable
            fileData.AddVariable("MyVariable1", "MyValue");

            // Modify it
            fileData.ModifyVariable("MyVariable1", "newValue");

            // Delete it
            fileData.RemoveVariable("MyVariable1"); 
        }
    }
}
```

### Usage in .atrc
```
! .atrc file
! create a new block with []
! variable, key or block names can't contain: !, % or ,
%variable%=value
<%private_variable%=value
[blockname]
key1=value
variable_list=%variable%, %private_variable%
```