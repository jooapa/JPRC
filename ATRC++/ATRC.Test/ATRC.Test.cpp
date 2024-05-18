#include "ATRC.Test.h"
#include "./libs/include/ATRC.h"
#include <iostream>

int main()
{
    std::string filename = "teäåöst.atrc";
    ATRCFiledata *filedata = Read(filename, "utf-8");
    if (filedata) {
        std::cout << "Filename: " << filedata->Filename << std::endl;
        std::cout << "Encoding: " << filedata->Encoding << std::endl;
        // Clean up
        for(Block block : *filedata->Blocks){
            std::cout << "Block: '" << block.Name <<"'"<< std::endl;
            for(Key key : block.Keys){
                std::cout << "Key: '" << key.Name << "' = '" << key.Value <<"'"<< std::endl;
            }
        }
        for(Variable var : *filedata->Variables){
            std::cout << "Variable: '" << var.Name << "' = '" << var.Value <<"'"<< std::endl;
        }
        delete filedata;
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }


    wchar_t line[INSERTVAR_MAX] = L"Start: %*% %abc% %*%";
    const wchar_t *args[] = { L"Hello,", L"world!", NULL };
    InsertVar(line, args);
    std::wcout << line << std::endl; // Output: Start: Hello, world!

    return 0;
}
