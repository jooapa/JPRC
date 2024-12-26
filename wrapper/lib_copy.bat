@echo off
mkdir lib
cd .. 
cd ATRC_2.0.0
xcopy /s /y /i .\Windows\*.* ..\wrapper\lib\
xcopy /s /y /i .\Linux\*.* ..\wrapper\lib\
xcopy /s /y /i .\macOS\*.* ..\wrapper\lib\

