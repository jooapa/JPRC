@echo off
set "ARCH=%1"
set "GENERATOR=%2"
if "%ARCH%" == "" (
    echo Usage: %0 ^{x86^|x64^} ^{Visual Studio 17 2022^| Ninja ^| Other generators^}
    exit /b 1
)
if "%GENERATOR%" == "" (
    set "GENERATOR=Visual Studio 17 2022"
)

setlocal
cd /D %~dp0


if "%ARCH%" == "x86" (
    set "ARCH_DIR=Win32"
    set "ARCH_FLAG=-A Win32"
) else (
    set "ARCH_DIR=x64"
    set "ARCH_FLAG=-A x64"
)

if NOT EXIST .\out\%ARCH_DIR% GOTO :SKIP_CLEAN
echo Building for "%ARCH%" architecture using "%GENERATOR%" generator
FINDSTR "%GENERATOR%" .\out\%ARCH_DIR%\CMakeCache.txt >NUL
if %errorlevel% NEQ 0 (
    if %errorlevel% EQU 2 (
        echo Bad syntax given, exiting...
        GOTO EXIT
    )
    RMDIR /S /Q .\out\%ARCH_DIR%
)

:SKIP_CLEAN

if "%GENERATOR%" == "Ninja" (
    call cmake -S . -B .\out\test -G "Ninja" -DATRC_BUILD_TESTS=ON
    call :ERROR_HANDLER
) else (
    call cmake -S . -B out\%ARCH_DIR% -DCMAKE_BUILD_TYPE=Debug -G "%GENERATOR%" %ARCH_FLAG% -DATRC_BUILD_TESTS=ON
    call :ERROR_HANDLER
)

call cmake --build out\%ARCH_DIR%
call :ERROR_HANDLER

COPY /Y .\ATRC.Test\test.atrc .\out\%ARCH_DIR%\ATRC.Test\Debug\test.atrc
CD .\out\%ARCH_DIR%\ATRC.Test\Debug
CALL .\ATRC.Test.exe

GOTO EXIT

:ERROR_HANDLER
if errorlevel 1 (
    echo Build failed
    GOTO EXIT
)

:EXIT
endlocal
exit /b %errorlevel%