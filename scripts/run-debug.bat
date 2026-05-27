@echo off
setlocal

pushd "%~dp0.."

if not exist build (
    echo Creating build directory and configuring CMake for Debug build...
    mkdir build
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -Wno-dev
    if errorlevel 1 (
        echo ERROR: CMake configuration failed!
        popd
        pause
        exit /b 1
    )
)

echo Building project...
cmake --build build --config Debug --parallel
if errorlevel 1 (
    echo ERROR: Build failed!
    popd
    pause
    exit /b 1
)

set "OUTPUT_DIR=build"
if exist "build\Debug" (
    set "OUTPUT_DIR=build\Debug"
)

echo.
echo Build succeeded! Running the program...
echo.

pushd %OUTPUT_DIR%
.\DungeonCrawler.exe
popd

popd
pause