# Macros and constants

## ATRC_INJECT, ATRC_CINJECT

### Usage

```cpp
#include <ATRC.h>
using namespace atrc;
int main()
{
    ATRC_FD fd = ATRC_FD("./test.atrc", ReadMode::ATRC_READ_ONLY);
    if (!fd.CheckStatus()) return 1;
    std::string res = "";
    std::string line = "%*%%*% %*%"
    ATRC_INJECT(line, "Hello", ",", "World!")
    std::cout << res << std::endl;
    return 0;
}
```

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