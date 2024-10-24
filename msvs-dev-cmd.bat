@ECHO OFF
:: Set the starting directory
setlocal
cd /d "%~dp0"
cd batch
call .\globals.bat
call .\msvs-dev-env.bat
cd ..
:: Restore the starting directory
cd /d "%~dp0"

:: Build the library
cd ATRC
call ..\batch\build.bat "Release"
cd ..

:: Build the test

cd ATRC.Test
if not exist .\libs mkdir .\libs
COPY /Y /B ..\ATRC\out\ATRC.dll .\libs\
COPY /Y /B ..\ATRC\out\ATRC.lib .\libs\

@REM Temporary location at .\libs\
if not exist .\libs\include mkdir .\libs\include
COPY /Y ..\ATRC\include\ATRC.hpp .\libs\include
if not exist .\out mkdir .\out
call ..\batch\build.bat "Debug"
call .\run.bat
