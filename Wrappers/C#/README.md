# ATRC C#

ATRC C# wrapper

To build the wrapper, create a lib directory inside this directory and paste Linux and Windows directories from the ATRC-<version>_<build_number> there

To build the library, please use <project_root>\scripts\build_cs.bat or run:
```bash
# Version: Take from <project_root>\VERSION
# Build Version: Take from <project_root>\BUILDNUMBER
dotnet publish ATRCWrapper -c Release /p:ATRCVersion="<version>" /p:ATRCBuildVersion="<build_version>"
```

To run the test program, use:
```bash
dotnet run --project ATRCTest
```