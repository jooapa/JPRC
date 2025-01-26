#include "ATRC.Test.h"
#include <iostream>
#include <ATRC.h>
using namespace atrc;
void cpp_main(void)
{
    // ATRC_FD fd = ATRC_FD("out/linux-x64-debug/build/ATRC.Test/test.atrc");
    ATRC_FD fd = ATRC_FD("out/win_64/ATRC.Test/Debug/test.atrc");
    
    if (!fd.CheckStatus()) {
        return;
    }
    std::cout << "OS: " << fd["os"] << std::endl;
    std::cout << "ARCH: " << fd["arch"] << std::endl;
    // C_PATRC_FD c_fd = fd.ToCStruct();
    // if (c_fd == NULL) {
    //     std::cout << "Failed to convert to C struct!" << std::endl;
    //     return;
    // }
    // std::cout << c_fd->variables[0].Name << std::endl;
    // Destroy_ATRC_FD(c_fd);
    // const char* line = fd["block_name"]["key"];
    // const char* args[] = {"Hello everyone", nullptr};
    // std::cout << "Before: " << line << std::endl;
    // char* res = InsertVar_S(line, args);
    // std::cout << "After: " << res << std::endl;
    // delete[] res;
}