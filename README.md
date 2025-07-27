# ATRC - Advanced Tagged Resource Configuration Library

ATRC is a robust **C++17 configuration library** with **full C compatibility**, designed for high-performance applications, game engines, and modular systems.

� **[Full Documentation](https://github.com/Antonako1/ATRC/blob/main/docs/)**

---

## 🔥 Key Features

- **Dual Language Support**: Full-featured C++ API with C fallback interface
- **Smart Preprocessor**: Conditional logic, file inclusion, platform detection
- **Dynamic Variables**: Runtime variable injection and substitution
- **Cross-Platform**: Windows and Linux support with architecture detection

---

## 🚀 Quick Start

### Basic Usage

```cpp
#include <ATRC.h>

int main() {
    atrc::ATRC_FD fd("./config.atrc", ATRC_READ_ONLY);
    if (!fd.CheckStatus()) return 1;

    fd.AddVariable("Project", "ATRC");
    fd.AddBlock("Settings");
    fd.AddKey("Settings", "Version", "2.3.0");

    std::cout << fd["Project"] << std::endl;             // → "ATRC"
    std::cout << fd["Settings"]["Version"] << std::endl; // → "2.3.0"
}
```

### Configuration Example

```ini
#!ATRC
%user%=admin
%env%=production

[ServerConfig]
#.IF WINDOWS
Platform=Windows
#.ELSE
Platform=Unix
#.ENDIF

Host=%user%@server.com
Environment=%env%
```

### Advanced Features

#### File Inclusion & Variable Injection
```ini
#!ATRC
# Include global definitions
#.INCLUDE globals.atrc

[Injection Example]
Inject1=Inject values from left to right with: %*%, %*%, %*%
Inject2=Inject values in defined order: %*2%, %*0%, %*1%
```

```cpp
// Variable injection in C++
std::string result = fd.InsertVar_S(fd["Injection Example"]["Inject1"], {"first", "second", "third"});
// Result: "Inject values from left to right with: first, second, third"

std::string ordered = fd.InsertVar_S(fd["Injection Example"]["Inject2"], {"A", "B", "C"});
// Result: "Inject values in defined order: C, A, B"
```

#### Enums & Raw Strings
```ini
#!ATRC
[EnumExample]
Key=Value       # Once read with ATRC_FD::GetEnumValue(), it will return 0

[RawStringExample]
# Use VAR when creating a raw string value for a variable
#.SR VAR
%RawVar%=Raw
STRING!
#.ER

# Use KEY when creating a raw string value for a key
#.SR KEY
RawString=This
is
a
raw string!
#.ER
```

```cpp
// Using enums and raw strings
int enumVal = fd.GetEnumValue("EnumExample", "Key");  // Returns 0
std::string raw = fd["RawStringExample"]["RawString"]; // Multi-line string preserved
```

---

## ⚙️ CMake Integration

```cmake
cmake_minimum_required(VERSION 3.8)
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_executable(MyProject "main.cpp")

# Add ATRC
set(ATRC_DIR "path/to/ATRC-2.3.0_{BUILD}/cmake")
find_package(ATRC REQUIRED)
target_include_directories(MyProject PRIVATE ${ATRC_INCLUDE_DIR})
target_link_libraries(MyProject PRIVATE ${ATRC_LIB_RELEASE})
```

---

## 🛠️ Build & Test

```bash
mkdir build
cd build
# If you want to compile the test program, switch "ATRC_BUILD_TESTS" to "ON"
cmake .. -G "Visual Studio 2022 17" -DATRC_BUILD_TESTS=ON
cmake --build .
```

---

## 📦 File Format

ATRC files support:

* **Variables**: `%key%=value` - Global runtime variables
* **Blocks**: `[Name]` - Configuration sections
* **Keys**: `Key=Value` - Settings within blocks
* **Comments**: `# Comment` - Documentation
* **Preprocessor**: `#.IF`, `#.INCLUDE`, etc. - Conditional logic

---

## 🧰 API Overview

| C++ Class/Function         | C Equivalent                     |
| -------------------------- | -------------------------------- |
| `atrc::ATRC_FD`            | `C_ATRC_FD`                      |
| `Block`, `Key`, `Variable` | `C_Block`, `C_Key`, `C_Variable` |

### Utility Functions

| Function             | Description                    |
| -------------------- | ------------------------------ |
| `atrc_to_list()`     | Splits value into string list  |
| `atrc_to_bool()`     | Converts to `true/false`       |
| `atrc_to_uint64_t()` | Parses to `uint64_t`           |
| `atrc_to_double()`   | Parses to `double`             |
| `atrc_math_exp()`    | Evaluates math expressions     |

See the **[Full Documentation](https://github.com/Antonako1/ATRC/blob/main/docs/)** for more and detailed information

---

## 🖥️ Supported Platforms

* ✅ Windows (x86, x64)
* ✅ Linux (x86, x64)

---

## 📄 License

ATRC is open-source under the [BSD-2-Clause license](LICENSE.txt).

Created by [Antonako1](https://github.com/antonako1)

