@ECHO OFF

ECHO Changing directory to ATRC
IF NOT pwd==%cd% (cd /d %~dp0) 

dotnet clean

CD ..\ATRC
dotnet clean
RMDIR /S /Q .\bin\
RMDIR /S /Q .\obj\
ECHO Running BuildNupkg.bat

CALL BuildNupkg.bat

SET "build_path=bin\Release"
SET "file_version=1.0.0"

ECHO Build path: %build_path%


ECHO ========================================
ECHO Changing directory to ATRC.Test

CD ..\ATRC.Test

RMDIR /S /Q .\bin\
RMDIR /S /Q .\obj\

ECHO Deleting ATRC from .nuget
RMDIR /S /Q C:\Users\%USERNAME%\.nuget\packages\atrc\

ECHO Copying ATRC to libs\
RMDIR /S /Q .\libs
RMDIR /S /Q .\libs
MKDIR libs
DEL /Q /F ".\libs\ATRC.%file_version%.nupkg"
COPY /Y /B "..\ATRC\%build_path%\ATRC.%file_version%.nupkg" .\libs
@REM COPY /Y /B "..\ATRC\%build_path%\net8.0\ATRC.dll" libs
dotnet restore


ECHO Running ATRC.Test
ECHO ========================================
dotnet run
ECHO ========================================