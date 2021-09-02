@ECHO OFF

if %1.==. (
	set nugetfeedpath="%UserProfile%\.nuget\feed"
) else (
	set nugetfeedpath="%1"
)
ECHO \--------------------------------------------------------------\
ECHO \                       Cleaning Folders                       \
ECHO \--------------------------------------------------------------\
ECHO

ECHO Cleaning NuGet folder
rmdir /S /Q out\nuget
ECHO Cleaning Build folders
rmdir /S /Q build\x86\debug
rmdir /S /Q build\x86\release
rmdir /S /Q build\x64\debug
rmdir /S /Q build\x64\release
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

mkdir build\x86\debug
ECHO Installing Conan Packages
conan install . -if build\x86\debug -s arch=x86 -s build_type=Debug --build=missing
ECHO Generating CMake Files
cmake -S . -B build\x86\debug -DCMAKE_BUILD_TYPE=Debug -A Win32
ECHO Building with CMake
cmake --build build\x86\debug --target aviationcalc
ECHO 

ECHO \--------------------------------------------------------------\
ECHO \                    Building x86 (Release)                    \
ECHO \--------------------------------------------------------------\
ECHO

mkdir build\x86\release
ECHO Installing Conan Packages
conan install . -if build\x86\release -s arch=x86 -s build_type=Release --build=missing
ECHO Generating CMake Files
cmake -S . -B build\x86\release -DCMAKE_BUILD_TYPE=Release -A Win32
ECHO Building with CMake
cmake --build build\x86\release --target aviationcalc
ECHO 

ECHO \--------------------------------------------------------------\
ECHO \                     Building x64 (Debug)                     \
ECHO \--------------------------------------------------------------\
ECHO

mkdir build\x64\debug
ECHO Installing Conan Packages
conan install . -if build\x64\debug -s arch=x86_64 -s build_type=Debug --build=missing
ECHO Generating CMake Files
cmake -S . -B build\x64\debug -DCMAKE_BUILD_TYPE=Debug -A x64
ECHO Building with CMake
cmake --build build\x64\debug --target aviationcalc
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                    Building x64 (Release)                    \
ECHO \--------------------------------------------------------------\
ECHO

mkdir build\x64\release
ECHO Installing Conan Packages
conan install . -if build\x64\release -s arch=x86_64 -s build_type=Release --build=missing
ECHO Generating CMake Files
cmake -S . -B build\x64\release -DCMAKE_BUILD_TYPE=Release -A x64
ECHO Building with CMake
cmake --build build\x64\release --target aviationcalc
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                    Packing NuGet Package                     \
ECHO \--------------------------------------------------------------\
ECHO

cd out\nuget
ECHO Fetching NuGet executable
powershell -Command "Invoke-WebRequest https://dist.nuget.org/win-x86-commandline/latest/nuget.exe -OutFile nuget.exe"
ECHO Packing Package
nuget.exe pack .
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                   Installing NuGet Package                   \
ECHO \--------------------------------------------------------------\
ECHO

CALL pack-nuget.bat %nugetfeedpath%

cd ..\..
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                      Package Installed                       \
ECHO \--------------------------------------------------------------\
ECHO
ECHO Add %nugetfeedpath% to your Visual Studio NuGet Package Feed.