@ECHO OFF

if %1.==. (
	set nugetfeedpath="%UserProfile%\.nuget\feed"
) else (
	set nugetfeedpath=%1
)

ECHO Fetching NuGet executable
powershell -Command "Invoke-WebRequest https://dist.nuget.org/win-x86-commandline/latest/nuget.exe -OutFile nuget.exe"
ECHO Packing Package
nuget.exe pack .
ECHO

ECHO Deleting existing package
nuget.exe delete ${NUGET_PACKAGE_NAME} ${CMAKE_PROJECT_VERSION} -Source %nugetfeedpath% -NonInteractive
ECHO Installing new package
nuget.exe init . %nugetfeedpath%