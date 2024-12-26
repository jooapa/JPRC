using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using ATRC;


namespace ATRC.Test
{
    class Program {
        static void Main(string[] args) {
            IntPtr handle = ATRC.Read("test.atrc");
            bool exists = ATRC.DoesExistVariable(fileData, "testVariable");
            Console.WriteLine($"Variable exists: {exists}");
        }
    }
}