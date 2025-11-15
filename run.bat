@echo off
setlocal enabledelayedexpansion

git submodule update --init --recursive

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug || (
    pause
    exit /b 1
)
cmake --build build --config Debug || (
    pause
    exit /b 1
)

echo Build succeeded! Running the program...
echo.
.\build\Debug\DungeonCrawler.exe

pause