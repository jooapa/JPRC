:: Following required in PATH
:: debian, 7z, Windows build tools, Strawberry Perl

@echo off
setlocal

REM Define the project root and output folder
cd /D %~dp0
cd ..
set PROJECT_ROOT=%cd%
set FOLDER=ATRC_2.2.0
set OUTPUT_DIR=%PROJECT_ROOT%\%FOLDER%

REM Clear the previous output folder
RMDIR /S /Q "%OUTPUT_DIR%"
DEL /Q/S/F %FOLDER%.zip
DEL /Q/S/F %FOLDER%.tar
DEL /Q/S/F %FOLDER%.tar.gz
DEL /Q/S/F %FOLDER%.7z

REM Define subdirectories for the file structure
mkdir "%OUTPUT_DIR%\cmake"
mkdir "%OUTPUT_DIR%\docs"
mkdir "%OUTPUT_DIR%\include"
mkdir "%OUTPUT_DIR%\Linux\x64\Debug"
mkdir "%OUTPUT_DIR%\Linux\x64\Release"
mkdir "%OUTPUT_DIR%\Linux\x86\Debug"
mkdir "%OUTPUT_DIR%\Linux\x86\Release"
mkdir "%OUTPUT_DIR%\Windows\x64\Debug"
mkdir "%OUTPUT_DIR%\Windows\x64\Release"
mkdir "%OUTPUT_DIR%\Windows\Win32\Release"
mkdir "%OUTPUT_DIR%\Windows\Win32\Debug"

REM Generate documentation
cd "%PROJECT_ROOT%\docs"
call create_compiled_html.bat

REM Compile Windows builds
cd "%PROJECT_ROOT%\scripts"
SET "PATH=%PATH%;C:\Strawberry\c\bin;"
SET "PATH=%PATH%;C:\Program Files\7-Zip;"
call build_windows.bat
if errorlevel 1 exit /b 1

REM Run Linux builds using WSL or similar
ECHO Building for Linux...
cd "%PROJECT_ROOT%\scripts"
call debian run "./build_linux.sh"

REM Copy documentation
copy "%PROJECT_ROOT%\docs\*" "%OUTPUT_DIR%\docs" /Y
copy "%PROJECT_ROOT%\chm\*.chm" "%OUTPUT_DIR%\docs" /Y
del "%OUTPUT_DIR%\docs\*.bat"

REM Copy include files
copy "%PROJECT_ROOT%\ATRC\include\ATRC.h" "%OUTPUT_DIR%\include" /Y

REM Copy CMake configuration
copy "%PROJECT_ROOT%\cmake\ATRCConfig.cmake" "%OUTPUT_DIR%\cmake" /Y

REM Copy README
copy "%PROJECT_ROOT%\README.md" "%OUTPUT_DIR%" /Y

REM Compress the output folder into various formats
7z a -tzip "%PROJECT_ROOT%\%FOLDER%.zip" "%OUTPUT_DIR%\*" -r                > NUL
7z a -ttar "%PROJECT_ROOT%\%FOLDER%.tar" "%OUTPUT_DIR%\*" -r                > NUL
7z a -tgzip "%PROJECT_ROOT%\%FOLDER%.tar.gz" "%PROJECT_ROOT%\%FOLDER%.tar"  > NUL
7z a -t7z "%PROJECT_ROOT%\%FOLDER%.7z" "%OUTPUT_DIR%\*" -r                  > NUL

REM Clean up temporary files
del "%PROJECT_ROOT%\%FOLDER%.tar"

REM Completion message
echo All tasks completed successfully!
endlocal
