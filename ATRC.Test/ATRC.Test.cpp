// #include "ATRC.Test.h"
// #include <iostream>
// #include <ATRC.h>
// using namespace atrc;
// void cpp_main(void)
// {
//     ATRC_FD fd = ATRC_FD("file.atrc");
//     if (!fd.CheckStatus()) {
//         std::cout << "File parsed unsuccesfully!" << std::endl;
//         return;
//     }
//     std::cout << fd["var_name"] << std::endl;

//     C_PATRC_FD c_fd = fd.ToCStruct();
//     const char* line = fd["block_name"]["key"];
//     const char* args[] = {"Hello everyone", nullptr};
//     std::cout << "Before: " << line << std::endl;
//     char* res = InsertVar_S(line, args);
//     std::cout << "After: " << res << std::endl;
//     delete[] res;
// }

#include <iostream>
#include <ATRC.h>
int main() {
    atrc::ATRC_FD fd = atrc::ATRC_FD("file.atrc");

    if(!fd.CheckStatus()) {
        std::cout << "File parsed unsuccesfully!" << std::endl;
        return 1;
    }

    std::cout << fd["var_name"] << std::endl;
    if(fd["block_name"]["key"] == "") {
        std::cout << "Key not found!" << std::endl;
    }

    std::string line = fd["block_name"]["key"];
    std::vector<std::string> args = {"Hello everyone"};
    std::cout << "Before: " << line << std::endl;
    std::string res = fd.InsertVar_S(line, args);
    std::cout << "After: " << res << std::endl;
    
    return 0;
}