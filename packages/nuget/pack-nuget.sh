#!/bin/sh
if [ -z ${1+x} ]
then
	nugetfeedpath="$HOME/.nuget/feed"
else
	nugetfeedpath=$1
fi

echo "Deleting existing package"
dotnet nuget delete ${NUGET_PACKAGE_NAME} ${CMAKE_PROJECT_VERSION} -s $nugetfeedpath --non-interactive
echo "Installing new package"
dotnet nuget push "*.nupkg" -s $nugetfeedpath