#!/bin/bash
cd "$(dirname "$0")/.." || { echo "ERROR: Cannot change to project root"; exit 1; }

set -e
set -o pipefail

if [ ! -d build ]; then
    echo "Creating build directory..."
    mkdir build || { echo "ERROR: Failed to create build directory!"; exit 1; }
fi

echo "Configuring CMake for Release build..."
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release || { echo "ERROR: CMake configuration failed!"; exit 1; }

CPU_CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
echo "Building project using $CPU_CORES cores..."
cmake --build build --config Release -j"$CPU_CORES" || { echo "ERROR: Build failed!"; exit 1; }

if [ -d "build/Release" ]; then
    OUTPUT_DIR="build/Release"
else
    OUTPUT_DIR="build"
fi

echo
echo "Build succeeded! You can find the release build in '$OUTPUT_DIR'"