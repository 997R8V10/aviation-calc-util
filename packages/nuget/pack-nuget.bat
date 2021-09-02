@ECHO OFF

if %1.==. (
	set nugetfeedpath="%UserProfile%\.nuget\feed"
) else (
	set nugetfeedpath="%1"
)

ECHO Deleting existing package
nuget.exe delete ${NUGET_PACKAGE_NAME} ${CMAKE_PROJECT_VERSION} -Source %nugetfeedpath% -NonInteractive
ECHO Installing new package
nuget.exe init . %nugetfeedpath%