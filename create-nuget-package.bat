@ECHO OFF

if %1.==. (
	set nugetfeedpath="%UserProfile%\.nuget\feed"
) else (
	set nugetfeedpath="%1"
)

ECHO Cleaning Folders
rmdir /S/Q out\nuget
#rmdir /S/Q build\x86\debug
#rmdir /S/Q build\x86\release
#rmdir /S/Q build\x64\debug
#rmdir /S/Q build\x64\release

ECHO Building x86 (Debug)
mkdir build\x86\debug
conan install . -if build\x86\debug -s arch=x86 -s build_type=Debug --build=missing
cmake -S . -B build\x86\debug -DCMAKE_BUILD_TYPE=Debug -A Win32
cmake --build build\x86\debug --target aviationcalc

ECHO Building x86 (Release)
mkdir build\x86\release
conan install . -if build\x86\release -s arch=x86 -s build_type=Release --build=missing
cmake -S . -B build\x86\release -DCMAKE_BUILD_TYPE=Release -A Win32
cmake --build build\x86\release --target aviationcalc

ECHO Building x64 (Debug)
mkdir build\x64\debug
conan install . -if build\x64\debug -s arch=x86_64 -s build_type=Debug --build=missing
cmake -S . -B build\x64\debug -DCMAKE_BUILD_TYPE=Debug -A x64
cmake --build build\x64\debug --target aviationcalc

ECHO Building x64 (Release)
mkdir build\x64\release
conan install . -if build\x64\release -s arch=x86_64 -s build_type=Release --build=missing
cmake -S . -B build\x64\release -DCMAKE_BUILD_TYPE=Release -A x64
cmake --build build\x64\release --target aviationcalc

ECHO Packing NuGet Package
cd out\nuget
powershell -Command "Invoke-WebRequest https://dist.nuget.org/win-x86-commandline/latest/nuget.exe -OutFile nuget.exe"
nuget.exe pack .

ECHO Installing NuGet Package
nuget.exe delete pshivaraman.aviationcalcutil 0.0.1 -Source %nugetfeedpath% -NonInteractive
nuget.exe init . %nugetfeedpath%

cd ..\..

ECHO Add "%nugetfeedpath%" to your Visual Studio NuGet Package Feed.