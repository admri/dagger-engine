@echo off

git submodule update --init --recursive

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release || (
    pause
    exit /b 1
)
cmake --build build --config Release || (
    pause
    exit /b 1
)

echo Build succeeded^^! You can find the release build in '.\build\Release'.

pause