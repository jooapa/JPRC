using ATRC;

namespace Test
{
    public class Program
    {
        private static ATRC.Filehandler filehandler = new();
        private static ATRC.Filehandler.ATRCFileData fileData = new();
        public static void Main(string[] args)
        {   
            fileData = ATRC.Filehandler.Read("example.atrc");

        }
    }
}
