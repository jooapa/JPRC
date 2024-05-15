:: Set the starting directory
setlocal
set "VSCMD_START_DIR=%CD%"
cd /d "%VSCMD_START_DIR%"
set "TOOLSPATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools"
@rem not needed
if not defined DevEnvDir ( 
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
)
:: Restore the starting directory
cd /d "%VSCMD_START_DIR%"

:: Build the library
cd ATRC
call .\build.bat "Debug"
cd ..

:: Build the test

cd ATRC.Test
if not exist .\libs mkdir .\libs
COPY /Y /B ..\ATRC\out\ATRC.dll .\libs\

@REM Temporary location at .\libs\
COPY /Y ..\ATRC\out\atrc_export.h .\libs\
call .\build.bat "Debug"
call .\run.bat
