@ECHO OFF
cd /d %~dp0
call .\globals.bat
if not defined DevEnvDir ( 
    call "%TOOLSPATH%\VsDevCmd.bat"
)