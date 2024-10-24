#include "ATRC.Test.h"
#define INCLUDE_ATRC_STDLIB
#include <ATRC.hpp>
#include <iostream>
#include <string>
#include <memory>

int main()
{
    std::string filename = "test.ini";
    PATRC_FD fd = Read(filename, "utf-8", ".ini");
    fd->AutoSave = true;
    if (fd) {
        AddBlock(fd, "test");
        
        AddKey(fd, "test", "troll", "aaa");
        
        AddKey(fd, "test", "tehi", "hi");
        RemoveKey(fd, "test", "tehi");
        RemoveKey(fd, "hello", "ququ");

        AddKey(fd, "test", "tehi", "hi");
        ModifyKey(fd, "test", "tehi", "helloa");

        AddKey(fd, "hello", "ququ", "qaqa");

        ModifyKey(fd, "hello", "ququ", "1233");

        // RemoveBlock(fd, "test");

        std::vector<std::string> res = atrc_to_list(';', ReadKey(fd, "moro", "hello"));
        for(std::string &s : res){
            std::cout << s << "\n";
        }

        DeleteATRCFiledata(fd);
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }

    return 0;
}
