ATRCFileData methods:
 - AddBlock          -> Adds given block
 - RemoveBlock       -> Removes given block
 - BlockExists       -> Check if block exists
 - AddVariable       -> Adds given variable with its value
 - A_ReadVariable    -> Reads list variable, returning contents in a string[]
 - S_ReadVariable    -> Reads variable, returning contents in a string
 - ModifyVariable    -> Modify given variable with given value
 - RemoveVariable    -> Removes given variable
 - VariableExists    -> Checks whether variable exists
 - IsPrivateVariable -> Checks if variable is private
 - AddKey            -> Adds key to the given block
 - A_ReadKey         -> Reads list key, returning its contents in a string[]
 - S_ReadKey         -> Reads key, returning its contents in a string
 - ModifyKey         -> Modifies given key with the given value
 - RemoveKey         -> Remove the given key from given block
 - MoveKey           -> Move key from source to destination
 - KeyExists         -> Checks whether the key exists
 - KeyInsert         -> Insert an object array of variables or values in the order they will be shown in the file, for more information see the C# example 

Example program in C#
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

            object[] inserts = ["ATRC", "1.3.0", 7];
            Console.WriteLine(fileData.S_KeyInsert("shell", "abc", inserts)); // -> Hello from ATRC. This is version 1.2.1, try 7!

            inserts = ["ATRC", 22];
            string [] keys = fileData.A_KeyInsert("shell", "def", inserts);
            foreach(string key in keys){
                Console.WriteLine(key);
            } 
            /*->
            List try!%&
            from ATRC,
            try number: 22...
            */
            
            
            /* file.atrc ->
            %system32_path%=C:\Windows\System32
            [WinApps]
            test_key=%system32_path%\notepad.exe

            [shell] ! these will be used for insert testing
            abc = Hello from %*%. This is \!\%\&\, version %*%\, try %*%\!
            def = List try\!\%\&\,, from %*%\,, try number: %*%...
            */
        }
    }
}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Example resource fil in .atrc
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
! Add whitespace to the end or start of your value with &, can also be used to add whitespace anywhere. Everything is case-sensitive
! create a new block with []
! variable, key or block names cannot contain: !, %, & or ,
! variables are constants, and can't be redefined in the file
! you can reference variables by placing them inside your keys: This is a %variable_name% reference,
!  
%variable%=value
<%private_variable%=private_value !This can't be accessed, modified or read outside the file. 
                                  ! It will be shown only once when the file is parsed, you must edit these variables through the file

[blockname]
key1=value 
! This will be read as a string and must be read as such (S_Read<Key|Variable>)

key_list=value1, value2 
! This will be read as a string[] and must be read as such (A_Read<Key|Variable>)

variable_list=%variable%, %private_variable% 
! when reading this, csharp will show {"value", " private_value"}

insertable_key=Your current operating system is %*% 
! Insert a variable to the place of %*%, will not be modify the value

whitespace = hello 
! Key would be 'whitespace' and its key 'hello' 

whitespace2 = hello&
! Now the key would be 'whitespace2' and its key 'hello '

! If you want to include !, %, &, or , in your value, use \ before it
reserved_characters=this\, is \&\% reserved\!
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%