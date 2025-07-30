# ATRC++. A C++ wrapper for ATRC

ATRC++ is a single include file wrapper for ATRC.h that automatically manages memory and allows the usage of strings, vectors, etc...

C++17 is required

See [API C documenation](https://github.com/Antonako1/ATRC/blob/main/docs/c_api.md). The functions are the same, arguments differ.

## Basic Example

```cpp
#include <ATRC.hpp>

int main() {
    atrc::ATRC_FD fd = atrc::ATRC_FD("confs.atrc", ATRC_READ_ONLY);
    if(!fd.CheckStatus()) {
        return 1;
    }

    std::cout << fd["varname"] << std::endl;
    fd["varname"] = "new_val";
    std::cout << fd["varname"] << std::endl;
    fd.RemoveVariable("varname");
    return 0;
}

```