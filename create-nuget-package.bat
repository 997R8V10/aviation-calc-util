@ECHO OFF

if %1.==. (
	set nugetfeedpath="%UserProfile%\.nuget\feed"
) else (
	set nugetfeedpath="%1"
)

CALL build-all-windows.bat

ECHO \--------------------------------------------------------------\
ECHO \                       Cleaning Folders                       \
ECHO \--------------------------------------------------------------\
ECHO

ECHO Cleaning NuGet folder
rmdir /S /Q out\nuget\windows
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                    Packing NuGet Package                     \
ECHO \--------------------------------------------------------------\
ECHO

cd out\nuget\windows
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