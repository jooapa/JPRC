#!/bin/bash
build_path="bin/Release/net8.0/publish"
echo "Build path: $build_path"

echo "Changing directory to ATRC"
cd "$(dirname "$0")"

cd ../ATRC

echo "Building ATRC"
dotnet publish -c Release

echo "Changing directory to ATRC.Test"
cd ../ATRC.Test

echo "Copying ATRC.dll to libs/"
if [ ! -d "libs" ]; then
  mkdir libs
fi

cp "../ATRC/$build_path/ATRC.dll" libs

mv libs/ATRC.dll libs/ATRC.dll
echo "Running ATRC.Test"
dotnet run