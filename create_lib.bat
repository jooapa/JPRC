@ECHO OFF


cd /d %~dp0
cd ATRC
RMDIR /S /Q out
cd ..
setlocal
call .\batch\globals.bat
call .\batch\msvs-dev-env.bat
cd ..\ATRC
call ..\batch\build.bat Release
cd /d %~dp0
RMDIR libs-output /S /Q 
mkdir libs-output
mkdir libs-output\include\
mkdir libs-output\docs\
mkdir libs-output\libs\win\
mkdir libs-output\libs\linux\
COPY .\docs\docs.md .\libs-output\docs
copy .\ATRC\out\ATRC.dll .\libs-output\libs\win
copy .\ATRC\out\ATRC.lib .\libs-output\libs\win
copy .\ATRC\include\ATRC.hpp .\libs-output\include\
copy ..\LICENSE.txt .\libs-output\
cd ATRC
RMDIR /S /Q out
cd /d %~dp0
endlocal