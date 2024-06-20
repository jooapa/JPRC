#include "ATRC.Test.h"
#include "./libs/include/ATRC.h"
#include <iostream>
#include <string>
#include <memory>

inline std::string get_home_dir(std::string homePath_arg = "") {

    char* homePath = nullptr;
    std::string homeDir;
#ifdef _WIN32
    size_t len;
    _dupenv_s(&homePath, &len, "USERPROFILE");
    homeDir = homePath ? std::string(homePath) : "";
    free(homePath);
#elif __linux__
    homePath = getenv("HOME");
    homeDir = homePath ? std::string(homePath) : "";
#endif
    if(homeDir == "")
        throw std::runtime_error("Could not get home directory, please set the HOME environment variable.");
    return homeDir;
}


int main()
{
    std::string homeDir = get_home_dir();
    
    std::string filename = "test.atrc";
    std::unique_ptr<ATRCFiledata> filedata = Read(filename, "utf-8");
    if (filedata) {


        std::string contents = "";
        ReadKey(filedata.get(), "block", "test", contents);
        std::cout << "'" << contents << "'" << std::endl;
        
        std::string arg = "test1";
        std::string arg2 = "test2";
        const std::string *args[] = { &arg, &arg2};
        InsertVar(contents, args);
        std::cout << contents << std::endl;

    } else {
        std::cerr << "Failed to read filedata." << std::endl;
    }

    return 0;
}
