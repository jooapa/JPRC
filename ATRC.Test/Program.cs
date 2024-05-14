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
            fileData.ModifyKey("test", "test_key", "%system32_path%\\notepad.exe "); // Modify key value

            // Move key to other location
            fileData.AddBlock("WinApps");
            fileData.MoveKey("test", "WinApps", "test_key");
            fileData.RemoveBlock("test"); // Remove block

            Console.WriteLine("'"+fileData.S_ReadKey("WinApps", "test_key")+"'"); // Read key from WinApps block

            Console.WriteLine("'"+fileData.S_ReadKey("shell", "test")+"'"); // Get file contents
            string test = fileData.S_ReadKey("shell", "test");
            fileData.ModifyKey("WinApps", "test_key", test); // Modify key value
            Console.WriteLine("'"+fileData.S_ReadKey("WinApps", "test_key")+"'"); // Read key from WinApps block

            fileData.ModifyKey("WinApps", "test_key", "%system32_path%\\lol"); // Modify key value
            Console.WriteLine("'"+fileData.S_ReadKey("WinApps", "test_key")+"'"); // Read key from WinApps block
            fileData.ModifyVariable("system32_path", "test"); // Modify variable value
            Console.WriteLine("'"+fileData.S_ReadKey("WinApps", "test_key")+"'"); // Read key from WinApps block
        }
    }
}