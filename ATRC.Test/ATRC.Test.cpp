#include "ATRC.Test.h"
#include <iostream>
#include <ATRC.h>
using namespace atrc;
void cpp_main(void)
{
    std::cout << "ATRC Test Application" << std::endl;
    ATRC_FD fd = ATRC_FD("./test.atrc", ReadMode::ATRC_READ_ONLY);
    //if (!fd.CheckStatus()) {
    //    std::cerr << "Failed to read ATRC file. Please check the file path and format." << std::endl;
    //    return;
    //}
    fd.SetAutoSave(true);
    fd.SetWriteCheck(true);
    //
    std::cout << "Running C++ tests" << std::endl;
    std::cout << fd["PremadeBlock"]["PremadeValue"] << std::endl;
    
    fd.AddBlock("TestBlock");
    fd.AddKey("TestBlock", "TestKey", "TestValue");
    //
    fd.AddBlock("AnotherBlock");
    fd.AddKey("AnotherBlock", "AnotherKey", "AnotherValue");
    fd.AddKey("AnotherBlock", "AnotherKey2", "AnotherValue");

    fd.RemoveBlock("TestBlock");
    fd.RemoveBlock("AnotherBlock");

    fd.AddVariable("TestVariable", "TestValue");

}