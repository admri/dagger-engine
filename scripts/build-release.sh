#!/bin/bash
set -e
set -o pipefail

pushd "$(dirname "$0")/.." > /dev/null || { echo "ERROR: Cannot change to project root"; exit 1; }

if [ ! -d build ]; then
    echo "Creating build directory and configuring CMake for Release build..."
    mkdir build || { echo "ERROR: Failed to create build directory!"; popd > /dev/null; exit 1; }
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -Wno-dev || { echo "ERROR: CMake configuration failed!"; popd > /dev/null; exit 1; }
fi

echo "Building project..."
cmake --build build --config Release --parallel || { echo "ERROR: Build failed!"; popd > /dev/null; exit 1; }

if [ -d "build/Release" ]; then
    OUTPUT_DIR="build/Release"
else
    OUTPUT_DIR="build"
fi

echo
echo "Build succeeded! You can find the release build in '$OUTPUT_DIR'"

popd > /dev/null