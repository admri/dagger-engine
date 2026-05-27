$ErrorActionPreference = "Stop"

Push-Location $PSScriptRoot\..

if (-Not (Test-Path "build")) {
    Write-Host "Creating build directory and configuring CMake for Release build..." -ForegroundColor Cyan
    New-Item -ItemType Directory -Force -Path "build" | Out-Null
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -Wno-dev
}

Write-Host "Building project using ($env:NUMBER_OF_PROCESSORS) threads..." -ForegroundColor Cyan
cmake --build build --config Release --parallel

$OutputDir = if (Test-Path "build\Release") { "build\Release" } else { "build" }
Write-Host "`nBuild succeeded! Running the program...`n" -ForegroundColor Green
Set-Location $OutputDir
.\DungeonCrawler.exe

Pop-Location