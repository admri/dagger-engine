@echo off
setlocal

cd /d "%~dp0"
cd ..

echo Updating git submodules...
git submodule update --init --recursive
if errorlevel 1 (
    echo ERROR: Git submodule update failed!
    pause
    exit /b 1
)

if not exist build (
    echo Creating build directory...
    mkdir build
    if errorlevel 1 (
        echo ERROR: Failed to create build directory!
        pause
        exit /b 1
    )
)

echo Configuring CMake for Debug build...
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo Building project...
cmake --build build --config Debug
if errorlevel 1 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo Build succeeded! Running the program...
echo.

cd .\build\Debug
.\DungeonCrawler.exe

pause