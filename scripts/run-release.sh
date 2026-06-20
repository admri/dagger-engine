#!/bin/bash
set -e
set -o pipefail

echo ">>> Starting Release Build Process..."
"$(dirname "$0")/build-release.sh" || { echo "ERROR: Build step failed!"; exit 1; }

pushd "$(dirname "$0")/.." > /dev/null || exit 1

BUILD_DIR="build-release"

if [ -d "$BUILD_DIR/Release" ]; then
    OUTPUT_DIR="$BUILD_DIR/Release"
elif [ -d "$BUILD_DIR/src/Release" ]; then
    OUTPUT_DIR="$BUILD_DIR/src/Release"
elif [ -f "$BUILD_DIR/src/DungeonCrawler" ]; then
    OUTPUT_DIR="$BUILD_DIR/src"
else
    OUTPUT_DIR="$BUILD_DIR"
fi

echo ">>> Running the program..."
echo

pushd "$OUTPUT_DIR" > /dev/null
./DungeonCrawler
popd > /dev/null

popd > /dev/null