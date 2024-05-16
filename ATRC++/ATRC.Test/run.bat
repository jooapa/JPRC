@ECHO OFF
IF NOT pwd==%cd% (cd /d %~dp0) 
ECHO ========================================
.\out\ATRC.Test.exe
ECHO ========================================
cd ..