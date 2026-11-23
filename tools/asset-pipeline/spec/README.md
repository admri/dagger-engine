# Specification of the final project for relevant C# courses
(When modifying this document, please maintain the layout and structure and follow the inline instructions.)

## C# Courses selection
(Change `[ ]` to `[x]` for the courses you plan to use this final project for.)

- [x] NPRG035 (Programming in C# language | Programování v jazyce C#)
- [x] NPRG038 (Advanced C# Programming | Pokročilé programování v jazyce C#)
- [ ] NPRG057 (Advanced .NET Programming II | Pokročilé programování pro .NET II)
- [ ] NPRG064 (Programming user interfaces in .NET | Programování uživatelských rozhraní v .NET)

## Specification

### Asset Pipeline CLI Util for Dagger Engine

#### Motivation:

In modern game development, loading raw assets (like parsing JSON files or decompressing PNG images) at runtime introduces several CPU bottlenecks and memory overhead. The Dagger Engine (a custom C++ ECS engine) currently suffers from this issue. This CLI application serves as an Asset Pipeline that processes raw, human-readable assets (JSON, PNG, WAV) and compiles them into a higly optimized, binary formats. By shifting the heavy lifting (parsing, validation, texture packing, and dependency resolution) onto the build step, the runtime C++ engine can simply load data directly into memory, resulting in blazingly fast load times and optimized GPU draw calls.

#### Use Case Scenarios:

The tool is inteded for game developers and level designers using the Dagger Engine. It will be used as a pre-build step during development to automatically translate raw source assets into engine-ready binary bundles whenever a file modification is detected.

#### Main Features:

- **Incremental Build System:** Scans source directories and computes hashes of raw assets. It maintains a dependency graph and a cache to ensure only modified files (and their dependents) are recompiled, singnificantly speeding up the build process.

- **Texture Atlas Packer:** Implements a 2D Bin Packing algorithm (ideally MaxRects) to take individual 2D sprite PNGs and pack them into a single, optimized Texture Atlas. It automatically calculates the new source coordinates (`srcRect`) and injects them into the compiled component data which reduces GPU texture swapping.

- **Prefab Linter & Binary Converter:** Reads entity prefabs defined in JSON. First, it acts as a linter and validates the JSON against a predefined component schema to catch typos or invalid data types before the game runs. Then, it compiles these text files into a custom binary format which perfectly mirrors the memory layout of the C++ ECS POD structures.

- **Dependency Resolver & Manifest Generator:** Parses `level.json` files and performs a recursive graph traversal (DFS) to resolve all required dependencies (prefabs, textures, audio) for a specific level. It then automatically generates a minimal `manifest.json` for the engine. This eliminates the need for manual manifest creation.

- **Archive Compression:** Compresses the compiled level data into a single `.pack` archive using .NET's compression streams to reduce the game's footprint on the disk.

- **Multi-threading & Asynchronous Data Pipelines:** The build system will be implemented using a Producer-Consumer pattern via `System.Threading.Channels` and `System.Threading.Tasks`. A producer task will asynchronously scan the file system and compute hashes, feeding required build jobs (like "Compile Prefab X" or "Pack Atlas Y") into a thread-safe channel. A pool of consumer worker threads will then process these jobs in parallel, handling CPU-bound tasks (2D bin packing, JSON serialization, binary writing) concurrently without race conditions.

#### UI/UX:

The application will be a pure CLI tool. There will be no graphical user interface.

Expected arguments/commands:
- `build --source <path> --output <path>`: Compiles the asset directory.
- `watch --srouce <path> --output <path>`: Runs continuously, watches for filesystem changes and recompiles files on the fly.
- `clean`: Clears the build cache and output directory.

#### Persistence:

- **Input:** Reads raw game assets (`.json` for prefabs/levels, `.png` for textures, `.wav` for audio).

- **Output:** Writes binary entity files, compiled texture atlases, and `manifest.json` files.

- **Internal state:** Maintains a cache JSON/binary file in the project root storing the dependency graph and file hashes across executions to support the incremental build feature.

#### Libraries/Technologies:

- **.NET 8** (or newer).

- **ImageSharp:** For reading and decoding raw PNG files into memory buffers before bin-packing.

- **xUnit**: For unit testing.

#### Testing:

The application's core logic will be covered by unit tests (xUnit):

- **Bin Packing Algorithm:** Tested by mocking rectangle dimensions and asserting that the resulting atlas bounds are optimal and sprites do not overlap.

- **Dependency Resolver:** Tested by supplying a mock dependency graph (e.g., Level -> Player -> Weapon -> Sound) and asserting that the output manifest contains all required nodes without duplicates.

- **Linter & Binary Converter:** Tested by supplying incorrect JSONs (asserting proper Exceptions are thrown) and valid JSONs (asserting the generated byte arrays match the expected C++ memory layout offsets).
