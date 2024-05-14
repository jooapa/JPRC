@ECHO OFF
COPY /Y ..\README.txt Readme.md
@REM dotnet clean
ECHO Building ATRC.nupkg
dotnet pack -c Release
