#!/bin/bash
echo "Setting global variables..."

ROOTPATH="$(cd "$(dirname "$0")"; pwd)"
CMAKEPATH="cmake"
NINJAPATH="ninja"
COMPIlER_CXX_PATH="g++"
COMPILER_C_PATH="gcc"

export ROOTPATH TOOLSPATH CMAKEPATH NINJAPATH PROJECTNAME COMPILER_C_PATH COMPIlER_CXX_PATH

echo "Done."
