using ATRC;

namespace Test
{
    public class Program
    {
        private static ATRC.Filehandler.ATRCFileData fileData = new();
        public static void Main(string[] args)
        {   
            // Read example.atrc
            fileData.Read("example.atrc");
            // Create a new block
            fileData.AddBlock("test");
            // Create new keys there
            fileData.AddKey("test", "test1", "MyValue");
            string[] test = ["MyValueARR", "MyValue2ARR"];
            fileData.AddKey("test", "test2", test);

            // Modify the keys
            //fileData.ModifyKey("test", "test1", test);
            //fileData.ModifyKey("test", "test2", "MyValue3");

            // Read one key to console
            //string content = fileData.ReadKey("test", "test2");
            //Console.WriteLine(content);

            // Remove one key
            //fileData.RemoveKey("test", "test1");

            // Create a new block and move a key
            //fileData.AddBlock("safety");
            //fileData.MoveKey("test", "safety", "test2");


            
            // Remove a block
            fileData.RemoveBlock("test");

            // Create a new variable
            fileData.AddVariable("MyVariable1", "MyValue");
            fileData.AddVariable("MyVariable2", test);

            fileData.ModifyVariable("MyVariable2", "MyValue123");
            fileData.RemoveVariable("MyVariable2");

            Console.WriteLine("----");
            Console.WriteLine("----");
            Console.WriteLine("----");
            string content = fileData.S_ReadVariable("MyVariable1");
            Console.WriteLine(content);
            Console.WriteLine("----");
            fileData.AddVariable("MyVariable3", test);
            string[] content2 = fileData.A_ReadVariable("MyVariable3");
            foreach (string item in content2){
                Console.WriteLine(item);
            }
            Console.WriteLine("----");
            Console.WriteLine("----");
            Console.WriteLine("----");
            string key_content1 = fileData.S_ReadKey("kakka", "trol");
            Console.WriteLine(key_content1);
            Console.WriteLine("----");
            string[] key_content2 = fileData.A_ReadKey("kakka", "test");
            foreach (string item in key_content2){
                Console.WriteLine(item);
            }
            Console.WriteLine("----");
            Console.WriteLine("----");
            Console.WriteLine("----");
            fileData.ModifyVariable("MyVariable3", "test");

            fileData.MoveKey("kakka", "testerer", "hereTEST");

            fileData.ModifyKey("testerer", "hereTEST", "asdopifsdaiudfasuij");

            fileData.RemoveKey("testerer", "hereTEST");

            fileData.AddKey("testerer", "hereTEST", "asd");
        }
    }
}
