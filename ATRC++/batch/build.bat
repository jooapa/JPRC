@ECHO OFF
@REM IF NOT pwd==%cd% (cd /d %~dp0) 
call ..\batch\globals.bat
if not exist out (mkdir out)
cd out

setlocal enabledelayedexpansion
set "current_dir=%cd%"
set "last_chars=!current_dir:~-13!"
if "%last_chars%"=="ATRC.Test\out" (
    COPY /Y /B "..\libs\ATRC.dll" .
    COPY /Y /B "..\libs\ATRC.lib" .
)
endlocal


if(%1 == "Release") (
"C:\Windows\system32\cmd.exe" /c "%SYSTEMROOT%\System32\chcp.com 65001 >NUL && "%CMAKEPATH%\CMake\bin\cmake.exe"  -G "Ninja"  -DCMAKE_C_COMPILER:STRING="%COMPILERPATH%" -DCMAKE_CXX_COMPILER:STRING="%COMPILERPATH%" -DCMAKE_BUILD_TYPE:STRING="%1" -DCMAKE_INSTALL_PREFIX:PATH="./install/x64-release"   -DCMAKE_MAKE_PROGRAM="%NINJAPATH%\ninja.exe" ".." 2>&1"
cmake --build . --config %1
) else (
"C:\Windows\system32\cmd.exe" /c "%SYSTEMROOT%\System32\chcp.com 65001 >NUL && "%CMAKEPATH%\CMake\bin\cmake.exe"  -G "Ninja"  -DCMAKE_C_COMPILER:STRING="%COMPILERPATH%" -DCMAKE_CXX_COMPILER:STRING="%COMPILERPATH%" -DCMAKE_BUILD_TYPE:STRING="Debug" -DCMAKE_INSTALL_PREFIX:PATH="./install/x64-debug"   -DCMAKE_MAKE_PROGRAM="%NINJAPATH%\ninja.exe" ".." 2>&1"
cmake --build . --config "Debug"
)

if %errorlevel% neq 0 exit /b %errorlevel%

cd ..