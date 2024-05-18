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
        std::cout << "Autosave: " << (filedata->AutoSave ? "True":"False") << std::endl;
        std::cout << "============================" << std::endl;
        filedata->AutoSave = true;
        std::string contents = "";
        ReadKey(filedata, "block1", "key", contents);
        std::cout << "Contents: " << contents << std::endl;
        ReadVariable(filedata, "var", contents);
        std::cout << "Contents: " << contents << std::endl;

        if(DoesExistVariable(filedata, "var")){
            std::cout << "Variable exists." << std::endl;
        } else {
            std::cout << "Variable does not exist." << std::endl;
        }

        if(IsPublic(filedata, "var")){
            std::cout << "Variable is public." << std::endl;
        } else {
            std::cout << "Variable is not public." << std::endl;
        }

        if(DoesExistKey(filedata, "block1", "key")){
            std::cout << "Key exists." << std::endl;
        } else {
            std::cout << "Key does not exist" << std::endl;
        }
        
        ReadKey(filedata, "block1", "key", contents);
        std::string arg1 = "tosi";
        const std::string *args[] = { &arg1 };
        InsertVar(contents, args);
        std::cout << "Contents: " << contents << std::endl;

        AddBlock(filedata, "block_test");
        RemoveBlock(filedata, "block3");
        for(auto block : *filedata->Blocks){
            std::cout << "Block: " << block.Name << std::endl;
        }

        DoesExistBlock(filedata, "block1") ? std::cout << "Block exists." << std::endl : std::cout << "Block does not exist." << std::endl;
        delete filedata;
    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }

    return 0;
}
