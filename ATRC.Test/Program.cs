using ATRC;
namespace Test
{
    public class Program
    {

        private static ATRC.Filehandler.ATRCFileData fileData = new(); // This will contain the file data
        
        static void Main(string[] args)
        {
            fileData.Read("file.atrc");
            fileData.ModifyKey("haha", "a", "a%test%a%test%");
            Debug.DebugConsole(fileData.S_ReadKey("haha", "a"));
        }
    }
}