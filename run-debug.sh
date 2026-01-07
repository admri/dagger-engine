#!/bin/bash
set -e
set -o pipefail

git submodule update --init --recursive

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

echo "Build succeeded! Running the program..."
cd ./build/Debug
./DungeonCrawler