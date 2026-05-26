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

echo Configuring CMake for Release build...
cmake -S . -B build
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

set "OUTPUT_DIR=build"
if exist "build\Release" (
    set "OUTPUT_DIR=build\Release"
)

echo.
echo Build succeeded! You can find the release build in %OUTPUT_DIR%.
echo.

pause