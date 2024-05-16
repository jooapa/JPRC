@ECHO OFF
IF NOT pwd==%cd% (cd /d %~dp0) 
cd ATRC
if exist out (RMDIR /Q /S out)
if exist libs (RMDIR /Q /S libs)
cd ..
cd ATRC.Test
if exist out (RMDIR /Q /S out)
if exist libs (RMDIR /Q /S libs)
cd..