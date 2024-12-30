#include "ATRC.Test.h"
#include <iostream>
#include <ATRC.h>

void cpp_main(void)
{
    std::cout << "Hello from C++!" << std::endl;
	atrc::ATRC_FD fd("test.atrc");
	std::string test = fd["test"];
	std::cout << test << " " << fd["test"] << std::endl;
}