#!/bin/bash
cd "$(dirname "$0")/.." || { echo "ERROR: Cannot change to project root"; exit 1; }

set -e
set -o pipefail

echo "Updating git submodules..."
git submodule update --init --recursive || { echo "ERROR: Git submodule update failed!"; exit 1; }

if [ ! -d build ]; then
    echo "Creating build directory..."
    mkdir build || { echo "ERROR: Failed to create build directory!"; exit 1; }
fi

echo "Configuring CMake for Demo build..."
cmake -S . -B build -DUSE_PREBUILT_VENDOR=ON -DCMAKE_BUILD_TYPE=Release || { echo "ERROR: CMake configuration failed!"; exit 1; }

CPU_CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
echo "Building project using $CPU_CORES cores..."
cmake --build build --config Release -j"$CPU_CORES" || { echo "ERROR: Build failed!"; exit 1; }

echo
echo "Build succeeded! Running the program..."

cd ./build/Release
./DungeonCrawler