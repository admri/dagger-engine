@echo off
setlocal

pushd "%~dp0.."

if not exist build (
    echo Creating build directory and configuring CMake for Release build...
    mkdir build
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -Wno-dev
    if errorlevel 1 (
        echo ERROR: CMake configuration failed!
        popd
        pause
        exit /b 1
    )
)

echo Building project...
cmake --build build --config Release --parallel
if errorlevel 1 (
    echo ERROR: Build failed!
    popd
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

popd
pause