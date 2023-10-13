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
ECHO

CALL build-all-windows.bat

ECHO \--------------------------------------------------------------\
ECHO \               Packing NuGet Package (Win-x86)                \
ECHO \--------------------------------------------------------------\
ECHO

cd out\nuget\win-x86
CALL pack-nuget.bat %nugetfeedpath%
cd ..\..\..
ECHO

ECHO \--------------------------------------------------------------\
ECHO \               Packing NuGet Package (Win-x64)                \
ECHO \--------------------------------------------------------------\
ECHO

cd out\nuget\win-x64
CALL pack-nuget.bat %nugetfeedpath%
cd ..\..\..
ECHO

ECHO \--------------------------------------------------------------\
ECHO \                     Packages Installed                       \
ECHO \--------------------------------------------------------------\
ECHO
ECHO Add %nugetfeedpath% to your Visual Studio NuGet Package Feed.