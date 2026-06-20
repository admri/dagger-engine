#!/bin/bash
set -e
set -o pipefail

pushd "$(dirname "$0")/.." > /dev/null || { echo "ERROR: Cannot change to project root"; exit 1; }

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory and configuring CMake for Debug build..."
    mkdir "$BUILD_DIR" || { echo "ERROR: Failed to create build directory!"; popd > /dev/null; exit 1; }
    cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -Wno-dev || { echo "ERROR: CMake configuration failed!"; popd > /dev/null; exit 1; }
fi

THREADS=1
if command -v nproc > /dev/null; then
    THREADS=$(nproc)
elif command -v sysctl > /dev/null; then
    THREADS=$(sysctl -n hw.ncpu)
fi

echo "Building project using ($THREADS) threads..."
cmake --build "$BUILD_DIR" --config Debug --parallel "$THREADS" || { echo "ERROR: Build failed!"; popd > /dev/null; exit 1; }

if [ -d "$BUILD_DIR/Debug" ]; then
    OUTPUT_DIR="$BUILD_DIR/Debug"
elif [ -d "$BUILD_DIR/src/Debug" ]; then
    OUTPUT_DIR="$BUILD_DIR/src/Debug"
elif [ -f "$BUILD_DIR/src/DungeonCrawler" ]; then
    OUTPUT_DIR="$BUILD_DIR/src"
else
    OUTPUT_DIR="$BUILD_DIR"
fi

echo
echo "Build succeeded! Running the program..."
echo

pushd "$OUTPUT_DIR" > /dev/null
./DungeonCrawler
popd > /dev/null

popd > /dev/null