@ECHO OFF
cd /d %~dp0
setlocal
.\batch\build.bat Release
cd /d %~dp0
if not exist libs mkdir libs
copy ..\ATRC\out\ATRC.dll .\libs
copy ..\ATRC\out\ATRC.lib .\libs