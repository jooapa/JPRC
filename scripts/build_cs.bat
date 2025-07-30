@echo off
setlocal
chcp 65001
cd /D %~dp0
call "global.bat"
cd ..
cd Wrappers\C#

if not exist lib mkdir lib
echo Copying libraries to lib directory...
XCOPY %OUTPUT_DIR%\Windows\* lib\Windows\ /E /I /Y
XCOPY %OUTPUT_DIR%\Linux\* lib\Linux\ /E /I /Y

set PROJECT=ATRC\ATRC.csproj

rmdir /S /Q ATRC\nupkg

set PACKAGE_NAME=ATRC-%VERSION%_%BUILD_VERSION%.nupkg
echo Building package: %PACKAGE_NAME%
dotnet pack ATRC -c Release /p:Version=%VERSION%
ren ATRC\nupkg\ATRC.*.nupkg %PACKAGE_NAME%
endlocal