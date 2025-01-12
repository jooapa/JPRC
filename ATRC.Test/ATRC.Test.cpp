#include "ATRC.Test.h"
#include <iostream>
#include <ATRC.h>
using namespace atrc;
void cpp_main(void)
{
    std::cout << "Hello from C++!" << std::endl;
	atrc::ATRC_FD fd("test.atrc");
	if (!fd.CheckStatus()) {
		std::cout << "File is empty!" << std::endl;
		return;
	}
	std::cout << "fd" << std::endl;
	std::string test = fd["test"]["test"];
	fd["test"]["test"] = "112233";
	std::cout << test << "|" << fd["test"] << "|" << fd.ReadKey("test", "test") << std::endl;
	fd["test"]["test"] >> "test 189";
	std::cout << fd["test"]["test"] << std::endl;

	C_ATRC_FD *c_fd = fd.ToCStruct();

	std::cout << "C Filedata: " << c_fd->Filename << std::endl;

	atrc::ATRC_FD fd2(c_fd);
	Destroy_ATRC_FD(c_fd);
	if(!fd2.CheckStatus()){
		std::cout << "File is empty!" << std::endl;
		return;
	}
	std::cout << fd2["test"]["test"] << std::endl;
}