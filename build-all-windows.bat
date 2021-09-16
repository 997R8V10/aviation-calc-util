@ECHO OFF

ECHO \--------------------------------------------------------------\
ECHO \                       Cleaning Folders                       \
ECHO \--------------------------------------------------------------\
ECHO

ECHO Cleaning Build folders
rmdir /S /Q build\windows\x86\debug
rmdir /S /Q build\windows\x86\release
rmdir /S /Q build\windows\x64\debug
rmdir /S /Q build\windows\x64\release
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                      Installing ecCodes                      \
ECHO \--------------------------------------------------------------\
ECHO
conan export eccodes

ECHO \--------------------------------------------------------------\
ECHO \                     Building x86 (Debug)                     \
ECHO \--------------------------------------------------------------\
ECHO

mkdir build\windows\x86\debug
ECHO Installing Conan Packages
conan install . -if build\windows\x86\debug -s arch=x86 -s build_type=Debug --build=missing
ECHO Generating CMake Files
cmake -S . -B build\windows\x86\debug -DCMAKE_BUILD_TYPE=Debug -A Win32
ECHO Building with CMake
cmake --build build\windows\x86\debug --target aviationcalc
ECHO 

ECHO \--------------------------------------------------------------\
ECHO \                    Building x86 (Release)                    \
ECHO \--------------------------------------------------------------\
ECHO

mkdir build\windows\x86\release
ECHO Installing Conan Packages
conan install . -if build\windows\x86\release -s arch=x86 -s build_type=Release --build=missing
ECHO Generating CMake Files
cmake -S . -B build\windows\x86\release -DCMAKE_BUILD_TYPE=Release -A Win32
ECHO Building with CMake
cmake --build build\windows\x86\release --target aviationcalc
ECHO 

ECHO \--------------------------------------------------------------\
ECHO \                     Building x64 (Debug)                     \
ECHO \--------------------------------------------------------------\
ECHO

mkdir build\windows\x64\debug
ECHO Installing Conan Packages
conan install . -if build\windows\x64\debug -s arch=x86_64 -s build_type=Debug --build=missing
ECHO Generating CMake Files
cmake -S . -B build\windows\x64\debug -DCMAKE_BUILD_TYPE=Debug -A x64
ECHO Building with CMake
cmake --build build\windows\x64\debug --target aviationcalc
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                    Building x64 (Release)                    \
ECHO \--------------------------------------------------------------\
ECHO

mkdir build\windows\x64\release
ECHO Installing Conan Packages
conan install . -if build\windows\x64\release -s arch=x86_64 -s build_type=Release --build=missing
ECHO Generating CMake Files
cmake -S . -B build\windows\x64\release -DCMAKE_BUILD_TYPE=Release -A x64
ECHO Building with CMake
cmake --build build\windows\x64\release --target aviationcalc
ECHO