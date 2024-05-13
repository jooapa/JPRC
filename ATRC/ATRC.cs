namespace ATRC
{
    // Globals

    public static class Debug{
        public static void DebugConsole(params object[] args) {
            string _output = "";
            foreach (var arg in args) {
                _output += arg.ToString() + " ";
            }
            Console.WriteLine("ATRC DEBUG:|" + _output);
        }
    }
}
