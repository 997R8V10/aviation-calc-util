#!/bin/sh
echo "\\--------------------------------------------------------------\\"
echo "\\                       Cleaning Folders                       \\"
echo "\\--------------------------------------------------------------\\"
echo ""

echo "Cleaning Build folders"
rm -rf build/linux/x64/release
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                      Installing ecCodes                      \\"
echo "\\--------------------------------------------------------------\\"
echo ""
conan export eccodes
conan export .

echo "\\--------------------------------------------------------------\\"
echo "\\                    Building x64 (Release)                    \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x64/release
echo "Installing Conan Packages"
conan install . -if build/linux/x64/release -s arch=x86_64 -s build_type=Release --build=missing
if [ $? -eq 0 ]; then
  echo "Generating CMake Files"
  cmake -S . -B build/linux/x64/release -DCMAKE_BUILD_TYPE=Release -DNUGET_ARCH=linux-x64
fi
if [ $? -eq 0 ]; then
  echo "Building with CMake"
  cmake --build build/linux/x64/release --target aviationcalc
fi
echo ""
