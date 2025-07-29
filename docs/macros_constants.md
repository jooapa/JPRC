# Macros and constants

## ATRC_INJECT

```c
#include <ATRC.h>
#include <stdio.h>

int main() {
    PATRC_FD fd = Create_Empty_ATRC_FD();
    if(fd == NULL) return 0;
    const char res* = NULL;
    const char line* = "%*%%*% %*%";
    ATRC_CINJECT(line, "Hello", ",", "World!")
    printf(res);
    free(res);
    Destroy_ATRC_FD(fd);
    return 0;
}
```