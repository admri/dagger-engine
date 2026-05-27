#!/bin/bash
set -e
set -o pipefail

pushd "$(dirname "$0")/.." > /dev/null || { echo "ERROR: Cannot change to project root"; exit 1; }

if [ ! -d build ]; then
    echo "Creating build directory and configuring CMake for Debug build..."
    mkdir build || { echo "ERROR: Failed to create build directory!"; popd > /dev/null; exit 1; }
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -Wno-dev || { echo "ERROR: CMake configuration failed!"; popd > /dev/null; exit 1; }
fi

echo "Building project using ($NUMBER_OF_PROCESSORS) threads..."
cmake --build build --config Debug --parallel || { echo "ERROR: Build failed!"; popd > /dev/null; exit 1; }

if [ -d "build/Debug" ]; then
    OUTPUT_DIR="build/Debug"
else
    OUTPUT_DIR="build"
fi

echo
echo "Build succeeded! Running the program..."
echo

pushd "$OUTPUT_DIR" > /dev/null
./DungeonCrawler
popd > /dev/null

popd > /dev/null