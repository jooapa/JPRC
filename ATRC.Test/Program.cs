using ATRC;
namespace Test
{
    public class Program
    {

        private static ATRC.Filehandler.ATRCFileData fileData = new(); // This will contain the file data
        
        static void Main(string[] args)
        {
            fileData.Read("file.atrc");
            string[] test = {"aq", "%test%", "a", "%test2a%", "a"};
            
            // fileData.ModifyVariable("test", "qa");
            
            // fileData.AddKey("haha", "aq", test);
            // fileData.AddKey("haha", "aq", "a%test%a%test2%a");
            
            // fileData.ModifyKey("haha", "a", test);
            fileData.ModifyKey("haha", "a", "a%test%a%testa2%a");
            // Debug.DebugConsole(fileData.S_ReadKey("haha", "a"));
        }
    }
}