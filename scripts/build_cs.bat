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

if "%1" == "DEBUG" (
    dotnet publish ATRCWrapper -c Debug -r win-x64 /p:ATRCVersion="%VERSION%" /p:ATRCBuildVersion="%BUILD_VERSION%" /p:ATRCFolder="%FOLDER%" 
) else (
    dotnet publish ATRCWrapper -c Release -r win-x64 /p:ATRCVersion="%VERSION%" /p:ATRCBuildVersion="%BUILD_VERSION%" /p:ATRCFolder="%FOLDER%"
)