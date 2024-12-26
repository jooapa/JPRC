using ATRCWrapper;

class Program {
    static void Main() {
        ATRCFiledata filedata = new ATRCFiledata("path/to/file");
        
        filedata.AddBlock("MyBlock");
        filedata.AddVariable("MyVar", "MyValue");
        
        Console.WriteLine(filedata.ReadVariable("MyVar"));
    }
}
