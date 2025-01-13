@echo off
setlocal

cd /D %~dp0
cd ..\docs
call create_compiled_html.bat
cd ..\scripts
SET "PATH=%PATH%;C:\Strawberry\c\bin;"
set "FOLDER=ATRC_2.1.0"
RMDIR /S /Q ..\%FOLDER%
call debian run "./loop.sh" 
copy ..\docs\* ..\%FOLDER%\docs 
copy ..\chm\*.chm ..\%FOLDER%\docs
del ..\%FOLDER%\docs\*.bat
copy ..\ATRC\include\ATRC.h ..\%FOLDER%\include

mkdir ..\%FOLDER%\cmake
copy ..\cmake\FindATRC.cmake ..\%FOLDER%\cmake
copy ..\README.md ..\%FOLDER%

set "PATH=%PATH%;C:\Program Files\7-Zip;"
7z a -tzip ..\%FOLDER%.zip ..\%FOLDER%
7z a -ttar ..\%FOLDER%.tar ..\%FOLDER%
7z a -tgzip ..\%FOLDER%.tar.gz ..\%FOLDER%.tar
7z a -t7z ..\%FOLDER%.7z ..\%FOLDER%
del ..\%FOLDER%.tar
setlocal