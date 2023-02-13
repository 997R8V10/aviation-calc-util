#!/bin/sh

if [ -z ${1+x} ]
then
	nugetfeedpath="$HOME/.nuget/feed"
else
	nugetfeedpath=$1
fi

# NuGet
echo "\\--------------------------------------------------------------\\"
echo "\\                       Cleaning Folders                       \\"
echo "\\--------------------------------------------------------------\\"
echo ""

echo "Cleaning NuGet folder"
rm -rf out/nuget/linux-x64
echo ""

# Build all
./build-all-linux.sh

echo "\\--------------------------------------------------------------\\"
echo "\\              Packing NuGet Package (Linux-x64)               \\"
echo "\\--------------------------------------------------------------\\"
echo ""

cd out/nuget/linux-x64
./pack-nuget.sh $nugetfeedpath
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\             Packing NuGet Package (Linux-arm64)              \\"
echo "\\--------------------------------------------------------------\\"
echo ""

cd out/nuget/linux-arm64
./pack-nuget.sh $nugetfeedpath
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                      Package Installed                       \\"
echo "\\--------------------------------------------------------------\\"
echo ""
echo "Add " $nugetfeedpath "to your Visual Studio NuGet Package Feed."