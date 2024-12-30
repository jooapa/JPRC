#include "ATRC.Test.h"
#include <iostream>
#include <ATRC.h>

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
	std::cout << test << " " << fd["test"] << " " << fd.ReadKey("test", "test") << std::endl;
}