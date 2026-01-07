@echo off
setlocal

cd /d "%~dp0"
cd ..

if not exist build (
    echo Creating build directory...
    mkdir build
    if errorlevel 1 (
        echo ERROR: Failed to create build directory!
        pause
        exit /b 1
    )
)

echo Configuring CMake for Demo build...
cmake -S . -B build -DUSE_PREBUILT_VENDOR=ON -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo Building project...
cmake --build build --config Release
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