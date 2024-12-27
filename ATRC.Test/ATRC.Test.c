#include "ATRC.Test.h"
#include <ATRC.h>
#include <stdio.h>

int main()
{    
	printf("Hello, World!\n");
	printf("Test: %d\n", test());
    PATRC_FD fd = CreateEmptyFiledata();
    if(fd == NULL) return 1;
    DestroyFiledata(&fd);
    return 0;
}
