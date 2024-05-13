# ATRC resource file library made with C#

Uses dotnet 8.0, propably works with other versions

### Usage

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
            fileData = ATRC.Filehandler.Read("file.atrc");
            
            // Creata new blocks 'test' and 'test2'
            fileData.AddBlock("test");
            fileData.AddBlock("test2");
            
            // Create two new keys 'key1' and 'key2' in block 'test' with values 'value1' and 'value2'
            fileData.AddKey("test", "key1", "value1");
            fileData.AddKey("test", "key2", "value2");

            string value = fileData.ReadKey("test", "key1"); // Read key 'key1' in block 'test'
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
