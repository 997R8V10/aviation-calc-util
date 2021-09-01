@ECHO OFF

ECHO Building x86 (Debug)
mkdir build\x86\debug
conan install . -if build\x86\debug -s arch=x86 -s build_type=Debug --build=missing
conan build . -bf build\x86\debug

ECHO Building x86 (Release)
mkdir build\x86\release
conan install . -if build\x86\release -s arch=x86 -s build_type=Release --build=missing
conan build . -bf build\x86\release

ECHO Building x64 (Debug)
mkdir build\x64\debug
conan install . -if build\x64\debug -s arch=x86_64 -s build_type=Debug --build=missing
conan build . -bf build\x64\debug

ECHO Building x64 (Release)
mkdir build\x64\release
conan install . -if build\x64\release -s arch=x86_64 -s build_type=Release --build=missing
conan build . -bf build\x64\release

ECHO Packing NuGet Package
cd out\nuget
powershell -Command "Invoke-WebRequest https://dist.nuget.org/win-x86-commandline/latest/nuget.exe -OutFile nuget.exe"
nuget.exe pack .

ECHO Installing NuGet Package
nuget.exe delete pshivaraman.aviationcalcutil 0.0.1 -Source %UserProfile%\.nuget\feed
nuget.exe init . -Source %UserProfile%\.nuget\feed