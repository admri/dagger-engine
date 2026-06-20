#!/bin/bash
set -e
set -o pipefail

pushd "$(dirname "$0")/.." > /dev/null || { echo "ERROR: Cannot change to project root"; exit 1; }

BUILD_DIR="build-release"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory and configuring CMake for Release build..."
    mkdir "$BUILD_DIR" || { echo "ERROR: Failed to create build directory!"; popd > /dev/null; exit 1; }
    cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -Wno-dev || { echo "ERROR: CMake configuration failed!"; popd > /dev/null; exit 1; }
fi

THREADS=1
if command -v nproc > /dev/null; then
    THREADS=$(nproc)
elif command -v sysctl > /dev/null; then
    THREADS=$(sysctl -n hw.ncpu)
fi

echo "Building project using ($THREADS) threads..."
cmake --build "$BUILD_DIR" --config Release --parallel "$THREADS" || { echo "ERROR: Build failed!"; popd > /dev/null; exit 1; }

if [ -d "$BUILD_DIR/Release" ]; then
    OUTPUT_DIR="$BUILD_DIR/Release"
elif [ -d "$BUILD_DIR/src/Release" ]; then
    OUTPUT_DIR="$BUILD_DIR/src/Release"
elif [ -f "$BUILD_DIR/src/DungeonCrawler" ]; then
    OUTPUT_DIR="$BUILD_DIR/src"
else
    OUTPUT_DIR="$BUILD_DIR"
fi

echo
echo "Build succeeded! You can find the release build in '$OUTPUT_DIR'"

popd > /dev/null