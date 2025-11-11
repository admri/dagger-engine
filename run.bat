@echo off
setlocal enabledelayedexpansion

git submodule update --init --recursive

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug

build\Debug\DungeonCrawler.exe

pause