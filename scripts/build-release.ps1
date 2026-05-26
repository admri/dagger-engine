$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot\..

Write-Host "Creating build directory..." -ForegroundColor Cyan
if (-Not (Test-Path "build")) {
    New-Item -ItemType Directory -Force -Path "build" | Out-Null
}

Write-Host "Configuring CMake for Release build..." -ForegroundColor Cyan
cmake -S . -B build

Write-Host "Building project..." -ForegroundColor Cyan
cmake --build build --config Release

$OutputDir = if (Test-Path "build\Release") { "build\Release" } else { "build" }
Write-Host "`nBuild succeeded! You can find the release build in: $OutputDir`n" -ForegroundColor Green