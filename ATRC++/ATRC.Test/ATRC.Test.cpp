#include "ATRC.Test.h"
#include "./libs/include/ATRC.h"
#include <iostream>

int main()
{
    std::string filename = "teäåöst.atrc";
    ATRCFiledata* filedata = Read(filename, "utf-8");
    if (filedata) {
        std::cout << "Filename: " << filedata->Filename << std::endl;
        std::cout << "Encoding: " << filedata->Encoding << std::endl;
        // Clean up
        delete filedata;
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }
    std::cout << "Hello World!" << std::endl;
    return 0;
}
