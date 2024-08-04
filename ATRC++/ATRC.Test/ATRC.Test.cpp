#include "ATRC.Test.h"
#include "./libs/include/ATRC.h"
#include <iostream>
#include <string>
#include <memory>

int main()
{
    std::string filename = "test.atrc";
    std::unique_ptr<ATRCFiledata> filedata = Read(filename, "utf-8");
    ATRCFiledata* rpfd = filedata.get();
    if (filedata) {
        // AddBlock(rpfd, "HELLO");
        // RemoveBlock(rpfd, "HELLO");

        AddVariable(rpfd, "MYTEST", "Hello, World! & %*% %!% %*0% %aa% qa%qa");
        // RemoveVariable(rpfd, "lol");
        ModifyVariable(rpfd, "MYTEST", "Hi, Earth?");
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }

    return 0;
}
