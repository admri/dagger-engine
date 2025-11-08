#!/bin/bash
git submodule update --init --recursive

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

./build/DungeonCrawler