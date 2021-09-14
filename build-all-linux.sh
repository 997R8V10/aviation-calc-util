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
echo "Running sudo apt install -y libc6-dev-i386 gcc-multilib g++-multilib libcurl4-openssl-dev"
echo "Please type your password when prompted, or install manually"
sudo apt install -y "libc6-dev-i386" "gcc-multilib" "g++-multilib" "libcurl4-openssl-dev"
conan export eccodes

echo "\\--------------------------------------------------------------\\"
echo "\\                     Building x86 (Debug)                     \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x86/debug
echo "Installing Conan Packages"
conan install . -if build/linux/x86/debug -s arch=x86 -s build_type=Debug --build=missing
echo "Building with Conan"
conan build . -bf build/linux/x86/debug
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                    Building x86 (Release)                    \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x86/release
echo "Installing Conan Packages"
conan install . -if build/linux/x86/release -s arch=x86 -s build_type=Debug --build=missing
echo "Building with Conan"
conan build . -bf build/linux/x86/release
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                     Building x64 (Debug)                     \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x64/debug
echo "Installing Conan Packages"
conan install . -if build/linux/x86/release -s arch=x86 -s build_type=Debug --build=missing
echo "Building with Conan"
conan build . -bf build/linux/x86/release
echo ""

echo "\\--------------------------------------------------------------\\"
echo "\\                    Building x64 (Release)                    \\"
echo "\\--------------------------------------------------------------\\"
echo ""

mkdir -p build/linux/x64/release
echo "Installing Conan Packages"
conan install . -if build/linux/x86/release -s arch=x86 -s build_type=Debug --build=missing
echo "Building with Conan"
conan build . -bf build/linux/x86/release
echo ""