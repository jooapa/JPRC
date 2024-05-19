#include "ATRC.Test.h"
#include "./libs/include/ATRC.h"
#include <iostream>

int main()
{
    std::string filename = "test.atrc";
    ATRCFiledata *filedata = Read(filename, "utf-8");
    if (filedata) {


        std::string contents = "";
        ReadKey(filedata, "block", "test", contents);
        std::cout << "'" << contents << "'" << std::endl;
        
        std::string arg = "test1";
        std::string arg2 = "test2";
        const std::string *args[] = { &arg, &arg2};
        InsertVar(contents, args);
        std::cout << contents << std::endl;

        delete filedata;
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }

    return 0;
}
