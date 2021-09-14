#!/bin/sh

if [ -z ${1+x} ]
then
	nugetfeedpath="$HOME/.nuget/feed"
else
	nugetfeedpath=$1
fi

# Build all
./build-all-linux.sh

# NuGet
echo "\\--------------------------------------------------------------\\"
echo "\\                       Cleaning Folders                       \\"
echo "\\--------------------------------------------------------------\\"
echo ""

echo "Cleaning NuGet folder"
rm -rf out/nuget/linux
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                    Packing NuGet Package                     \\"
echo "\\--------------------------------------------------------------\\"
echo ""

cd out/nuget/linux
echo "Packing Package"
dotnet pack .
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                   Installing NuGet Package                   \\"
echo "\\--------------------------------------------------------------\\"
echo ""

./pack-nuget.sh $nugetfeedpath

cd ../..
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                      Package Installed                       \\"
echo "\\--------------------------------------------------------------\\"
echo ""
echo "Add " + $nugetfeedpath + "to your Visual Studio NuGet Package Feed."