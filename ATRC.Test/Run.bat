@ECHO OFF

ECHO Changing directory to ATRC.Test
IF NOT pwd==%cd% (cd /d %~dp0) 

@REM Remove bin and obj just in case
RMDIR /S /Q .\bin\
RMDIR /S /Q .\obj\
RMDIR /S /Q .\libs\

CD ..\ATRC

RMDIR /S /Q .\bin\
RMDIR /S /Q .\obj\

@REM Build the nupkg
ECHO Running BuildNupkg.bat
CALL BuildNupkg.bat

@REM Set the build path and nupkg version
SET "build_path=bin\Release"
ECHO Build path: %build_path%

ECHO ========================================
ECHO Changing directory to ATRC.Test
CD ..\ATRC.Test

ECHO Deleting ATRC from .nuget
RMDIR /S /Q C:\Users\%USERNAME%\.nuget\packages\atrc\

ECHO Copying ATRC to libs\
@REM Remove libs, make libs and copy nupkg to libs
MKDIR libs
COPY /Y /B "..\ATRC\%build_path%\ATRC.1.3.2.nupkg" .\libs
dotnet restore

@REM Run test program
ECHO Running ATRC.Test
ECHO ========================================
dotnet run
ECHO ========================================
