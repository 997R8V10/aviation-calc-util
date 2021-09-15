#!/bin/sh
if [ -z ${1+x} ]
then
	nugetfeedpath="$HOME/.nuget/feed"
else
	nugetfeedpath=$1
fi

mkdir -p $nugetfeedpath

echo "Deleting existing package"
mono nuget.exe delete ${NUGET_PACKAGE_NAME} ${CMAKE_PROJECT_VERSION} -Source $nugetfeedpath -NonInteractive
echo "Installing new package"
mono nuget.exe init . $nugetfeedpath