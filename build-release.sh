#!/bin/bash
set -e
set -o pipefail

git submodule update --init --recursive

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -j

echo "Build succeeded! You can find the release build in './build/Release'."