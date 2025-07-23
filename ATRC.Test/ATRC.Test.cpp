#include "ATRC.Test.h"
#include <iostream>
#include <ATRC.h>
using namespace atrc;
void cpp_main(void)
{
    std::cout << "ATRC Test Application\n" << std::endl;
    ATRC_FD fd = ATRC_FD("./test.atrc", ReadMode::ATRC_READ_ONLY);
    if (!fd.CheckStatus()) {
        std::cerr << "Failed to read ATRC file. Please check the file path and format." << std::endl;
        return;
    }
    fd.SetAutoSave(true);
    fd.SetWriteCheck(true);

    std::cout << "Running C++ tests" << std::endl;
    std::cout << fd["PremadeBlock"]["PremadeValue"] << std::endl;
    
    fd.AddBlock("TestBlock");
    fd.AddKey("TestBlock", "TestKey", "TestValue");

    fd.AddBlock("AnotherBlock");
    fd.AddKey("AnotherBlock", "AnotherKey", "AnotherValue");
    fd.AddKey("AnotherBlock", "AnotherKey2", "AnotherValue");

    fd.RemoveBlock("TestBlock");
    fd.RemoveBlock("AnotherBlock");

    fd.AddVariable("TestVariable", "TestValue");

    
    std::string res = "";
    ATRC_INJECT(fd, "This is a test: %*%", "TestVariable");
    std::cout << res << std::endl;


    double math_calculated = atrc_std::atrc_math_exp(fd["PVD_OFFSET"]);
    std::cout << "Math calculated: " << math_calculated << std::endl;

    std::cout << atrc_std::atrc_math_exp(fd["JP_BMI"]) << std::endl;

    std::cout << fd["test3"] << std::endl << std::endl;
    std::cout << fd["raw_string"] << std::endl << std::endl;
    std::cout << fd["TESTBLOCK"]["TESTKEY"] << std::endl;
    std::cout << "Test completed successfully." << std::endl;
    fd.WriteCommentToTop("This is a comment at the top of the file.");
    fd.WriteCommentToBottom("This is a comment at the bottom of the file.");
}