#include "ATRC.Test.h"
#include <ATRC.h>


int main()
{
    ATRC_FD fd;
    Constructor_Pathed(fd, "test.atrc");
    ReadKey(&fd, "block", "key");
}
