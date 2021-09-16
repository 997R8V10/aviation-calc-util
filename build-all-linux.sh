#!/bin/sh
echo "\\--------------------------------------------------------------\\"
echo "\\                       Cleaning Folders                       \\"
echo "\\--------------------------------------------------------------\\"
echo ""

echo "Cleaning Build folders"
rm -rf build/linux/x86/debug
rm -rf build/linux/x86/release
rm -rf build/linux/x64/debug
rm -rf build/linux/x64/release
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                      Installing ecCodes                      \\"
echo "\\--------------------------------------------------------------\\"
echo ""
conan export eccodes

echo "\\--------------------------------------------------------------\\"
echo "\\                     Building x86 (Debug)                     \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x86/debug
echo "Installing Conan Packages"
conan install . -if build/linux/x86/debug -s arch=x86 -s build_type=Debug --build=missing
if [ $? -eq 0 ]; then
  echo "Generating CMake Files"
  cmake -S . -B build/linux/x86/debug -DCMAKE_BUILD_TYPE=Debug
fi
if [ $? -eq 0 ]; then
  echo "Building with CMake"
  cmake --build build/linux/x86/debug --target aviationcalc
fi
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                    Building x86 (Release)                    \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x86/release
echo "Installing Conan Packages"
conan install . -if build/linux/x86/release -s arch=x86 -s build_type=Release --build=missing
if [ $? -eq 0 ]; then
  echo "Generating CMake Files"
  cmake -S . -B build/linux/x86/release -DCMAKE_BUILD_TYPE=Release
fi
if [ $? -eq 0 ]; then
  echo "Building with CMake"
  cmake --build build/linux/x86/release --target aviationcalc
fi
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                     Building x64 (Debug)                     \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x64/debug
echo "Installing Conan Packages"
conan install . -if build/linux/x64/debug -s arch=x86_64 -s build_type=Debug --build=missing
if [ $? -eq 0 ]; then
  echo "Generating CMake Files"
  cmake -S . -B build/linux/x64/debug -DCMAKE_BUILD_TYPE=Debug
fi
if [ $? -eq 0 ]; then
  echo "Building with CMake"
  cmake --build build/linux/x64/debug --target aviationcalc
fi
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                    Building x64 (Release)                    \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x64/release
echo "Installing Conan Packages"
conan install . -if build/linux/x64/release -s arch=x86_64 -s build_type=Release --build=missing
if [ $? -eq 0 ]; then
  echo "Generating CMake Files"
  cmake -S . -B build/linux/x64/release -DCMAKE_BUILD_TYPE=Release
fi
if [ $? -eq 0 ]; then
  echo "Building with CMake"
  cmake --build build/linux/x64/release --target aviationcalc
fi
echo ""
