#include "ATRC.Test.h"
#define INCLUDE_ATRC_STDLIB
#include <ATRC.hpp>
#include <iostream>
#include <string>
#include <memory>

int main()
{
    std::string filename = "test.atrc";
    std::shared_ptr<ATRC_FD> fd = Read(filename, "utf-8");
    fd->AutoSave = true;
    if (fd) {
        AddBlock(fd, "block_name");
        AddKey(fd, "block_name", "key_name", "value");
        AddKey(fd, "block_name", "key_name2", "value");
        RemoveKey(fd, "block_name", "key_name2");
        ModifyKey(fd, "block_name", "key_name2", "new_value");
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }

    return 0;
}
