@ECHO OFF
call .\globals.bat
if not defined DevEnvDir ( 
    call "%TOOLSPATH%\VsDevCmd.bat"
)