#include "ATRC.Test.h"
#include <iostream>
#include "../Wrappers/C++/include/ATRC.hpp"

int cpp_main(void)
{
    std::cout << "ATRC Test Application\n" << std::endl;

    atrc::_ATRC_FD fd = atrc::_ATRC_FD("./ATRC.Test/test.atrc", ATRC_READ_ONLY);
    std::cout << fd["test"] << std::endl;

    return 0;
}