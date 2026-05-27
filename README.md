# Dagger + Delve

A modern, SDL3-based C++ engine (**Dagger**) and a dungeon crawler game (**Delve**).

## Prerequisites

Before building the project, ensure you have the following installed:

* **C++20 compatible compiler** (MSVC, GCC, or Clang)
* **Git**
* **CMake** (3.16 or higher)

### Linux Specific Dependencies
If you are building on Linux (Ubuntu/Debian), you will need basic build tools and development libraries for audio and window management:
```bash
sudo apt update
sudo apt install git cmake build-essential \
libx11-dev libxext-dev libxrandr-dev libxcursor-dev \
libxi-dev libxss-dev libxtst-dev libaudio-dev libasound2-dev
```

## Building the project

### Manual approach

This project uses standard CMake flow. Dependencies (like SDL3) are managed automatically and will be downloaded during the configuration step.

1. Clone the repository:

```bash
git clone https://gitlab.mff.cuni.cz/teaching/nprg041/2025-26/skoda/rihaada1.git
cd rihaada1/project
```

2. Configure the project:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

3. Build the project:

```bash
cmake --build build --config Release --parallel
```

4. Run the game:

- **Windows**: ```.\build\Release\DungeonCrawler.exe```

- **Linux**: ```./build/Release/DungeonCrawler```

### Build scripts

If you prefer an automated one-click solution, the `scripts` folder contains utilities to configure, build, and run the project automatically.

#### Windows (PowerShell):

```ps1
.\scripts\run-release.ps1
```

#### Windows (Batch):

```bat
.\scripts\run-release.bat
```

#### Linux (Bash):

```bash
./scripts/run-release.sh
```