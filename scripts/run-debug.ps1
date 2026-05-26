$ErrorActionPreference = "Stop"

Set-Location $PSScriptRoot\..

Write-Host "Creating build directory..." -ForegroundColor Cyan
if (-Not (Test-Path "build")) {
    New-Item -ItemType Directory -Force -Path "build" | Out-Null
}

Write-Host "Configuring CMake for Debug build..." -ForegroundColor Cyan
cmake -S . -B build

Write-Host "Building project..." -ForegroundColor Cyan
cmake --build build --config Debug

Write-Host "`nBuild succeeded! Running the program...`n" -ForegroundColor Green
Set-Location .\build\Debug
.\DungeonCrawler.exe