# ATRC - Advanced Tagged Resource Configuration Library

ATRC is a robust **C configuration library**, designed for high-performance applications, game engines, and modular systems.

� **[Full Documentation](https://github.com/Antonako1/ATRC/blob/main/docs/)**

---

## 🔥 Key Features

- **Smart Preprocessor**: Conditional logic, file inclusion, platform detection
- **Dynamic Variables**: Runtime variable injection and substitution
- **Cross-Platform**: Windows and Linux support with architecture detection

---

## 🚀 Quick Start

### Basic Usage

```c
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

```
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

```c
// Using enums and raw strings
```

---

## ⚙️ CMake Integration

```cmake
cmake_minimum_required(VERSION 3.8)
project(MyProject VERSION 1.0 LANGUAGES C)

set(CMAKE_CXX_STANDARD 17)
add_executable(MyProject "main.c")

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
cmake .. -DATRC_BUILD_TESTS=ON
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

## 🖥️ Supported Platforms

* ✅ Windows (x86, x64)
* ✅ Linux (x86, x64)

---

## 📄 License

ATRC is open-source under the [BSD-2-Clause license](LICENSE.txt).

Created by [Antonako1](https://github.com/antonako1)

