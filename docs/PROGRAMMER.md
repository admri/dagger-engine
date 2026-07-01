# **Dagger** Engine - Developer Guide

This document serves as an architectural guide for the **dagger** 2D game engine.

It explains the core design patterns, module structure, memory management, and inter-system communication used throughout the codebase.

---

## 1. Engine Structure & Module Interaction

The engine is strictly decoupled into distinct modules. Each module has a specific responsibility, ensuring that hardware wrappers do not mix with high-level game logic.

* **`core`**: The heart of the engine. It contains the *Entity-Component-System* foundation (`Entity.h`, `Component.h`, `Scene.h`), the main game loop (`Application.h`) and standard built-in components and systems.

* **`assets`**: Handles the data-driven instantiation and resource lifecycles. It is responsible for parsing JSON files (`AssetPreloader.h`, `PrefabFactory.h`) and safely storing resources in memory (`ResourceManager.h`, `AssetManager.h`).

* **`graphics` & `audio`**: Hardware-facing modules. These contain low-level wrappers (like `Renderer.h` or `AudioManager.h`) that abstract away the underlying multimedia libraries (e.g., SDL3, SDL_Mixer...). They have no knowledge of the ECS.

* **`input`**: Manages keyboard and mouse states (`Input.h`, `KeyCodes.h`), providing a clean interface for systems to query user actions.

* **`math` & `utils`**: Core utilities required across all modules. This includes custom math structures (`Vec2`, `PerlinNoise`) and hashing algorithms (`StringHash`).

## 2. Core Architecture: Entity-Component-System (ECS)

The engine avoids deep *OOP* inheritance hierarchies in favor of a Data-Oriented **ECS** architecture.
This strictly separates data from logic, which prevents tight coupling and unwanted problems tied to inheritance.

* **Entities:** Represented purely as unique identifiers (IDs). An `Entity` has no inherent logic or properties; it acts solely as a container for components.

* **Components (Data):** Plain Old Data (POD) structures. They hold state but no behavior. Examples include `TransformComponent` (positions) or built-in engine components like `SpriteComponent`.

* **Systems (Logic):** Stateless (...almost) classes that encapsulate all game logic. Every frame, a system queries the `Scene` for all entities possessing a specific signature (a combination of components) and processes them in contiguous memory batches.

**Example of querying entities within a system:**
```cpp
scene.forEachEntityWith<TransformComponent, SpriteComponent>(
    [&](dagger::Entity* e, auto& transform, auto& sprite) {
        // Logic executed for every renderable entity
    });
```

## 3. The Game Loop & Time Management

At the core of the `Application` is a robust game loop that decouples rendering framerate from game logic execution. This ensures deterministic behavior across different hardware.

* **Variable Time Step (`onUpdate` / Rendering):** The standard update loop calculates a variable delta time (`dt`) based on the actual time elapsed between frames. This is used for interpolation, animations, and rendering passes, ensuring smooth visuals regardless of hardware performance.
* **Fixed Time Step (`onFixedUpdate`):** To prevent physics tunneling and ensure consistent game mechanics, the engine utilizes an accumulator pattern. The configuration defines a specific Tick Rate (`tps`, e.g., 60 Ticks Per Second). The engine accumulates frame time and calls the fixed update strictly when the required time chunk (e.g., `1.0 / 60.0` seconds) has passed.

This timing system guarantees that a character moving at *100 pixels per second* will travel the exact same distance on a 30 Hz monitor as on a 144 Hz monitor.

## 4. Communication Protocols

To maintain high modularity, the engine uses two distinct communication patterns depending on the scope of the interaction.

### Inter-System Communication (EventBus)

Game logic systems (e.g., `ShootingSystem`, `HealthSystem`, `QuestSystem`) are completely isolated from one another. They communicate via a local *Publisher/Subscriber* pattern using the `EventBus` owned by the current `Scene`.

When a gameplay event occurs, a system broadcasts it:

```cpp
scene.getEventBus().emit<EnemyDiedEvent>(x, y, enemyScoreWorth);
```

Other systems can `subscribe` to this event. Every `Scene` has its own local `EventBus` which clears itself after the scene is destroyed.

### Hardware Core Services

Low-level hardware wrappers (`Renderer`, `AudioManager`, `Input`, or even `AssetManager`) are not ECS systems. They are core services accessed directly via the `Application` instance.

This avoids the overhead of event dispatching for high-frequency operations (like drawing textures or playing sounds).

```cpp
app.getAudio().play(soundHandle); // Direct command to the hardware wrapper
```

## 5. Asset Management & Serialization

`dagger` is designed as a **Data-Driven** engine. Textures, sounds, and entity configurations are not hardcoded in C++ but are loaded externally from *JSON* files.

### String Hashing & Resource Handles

Working with raw `std::string` during the game loop is highly inefficient. The engine utilizes a two-step optimization process:

1. `StringHash`: Raw strings (e.g. `"dungeon_theme"`) are hashed into a 32-bit integer using the FNV-1a algorithm. This is often evaluated at compile-time using the `""_hash` literal to ensure zero runtime overhead.

2. `ResourceHandle`: Components do not store file paths. The `AssetManager` loads a resource into memory and returns a lightweight *Generational Handle* (e.g. `TextureHandle`, `SoundHandle`). Systems use this handle for instantaneous array-index lookups during runtime (`assets.resolve(handle)`). This ensures fast asset access as well as pointer validity during the runtime.

### Prefabs & Deserialization

Entities are instantiated via Prefabs (JSON blueprints). The `PrefabFactory` handles the deserialization of these files. While primitive data (floats, ints) can be serialized automatically via macros, complex assets (like resolving a sound name to a runtime Handle) are defined via custom lambdas:

```cpp
PrefabFactory::registerComponent("WeaponComponent", 
    [](dagger::Entity* e, const nlohmann::json& j, dagger::AssetManager& assets) {
    auto& weapon = e->addComponent<WeaponComponent>();
    weapon = j.get<WeaponComponent>(); // Standard primitive loading
    
    if (j.contains("shootSound")) {
        // Translates string from JSON directly to a fast memory Handle
        weapon.shootSound = assets.get<Sound>(j["shootSound"].get<std::string>());
    }
});
```

## 6. Level Loading & Scene Population

The engine provides a robust, automated way to construct scenes from external data files via the `LevelLoader` subsystem (located in `core/`).

Rather than hardcoding entity instantiation, the engine relies on a pipeline:

1. **Manifest Loading**: Before a level starts, the `AssetManager` reads a manifest file to preload all necessary textures, audio files, and prefabs into memory. This prevents stuttering during gameplay.

2. **Level Parsing**: The `LevelLoader` parses a level configuration file (typically mapping data, tile layouts, and entity spawn points).

3. **Instantiation**: For each entity defined in the level data, the `LevelLoader` delegates the heavy lifting to the `PrefabFactory`. The factory clones the base JSON prefab, applies any level-specific overrides (like a unique `TransformComponent` position for a specific enemy), and registers the fully constructed `Entity` directly into the active `Scene`.

(Note: The exact JSON schemas for manifests and level maps are documented in the [USER.md](./USER.md) for content creators.)

## 7. Graphics & Shader Support

The graphics module (`graphics/`) is designed to decouple the ECS rendering systems (like `SpriteRenderSystem` or `UIRenderSystem`) from the underlying graphics API.

* **The `Renderer` Interface**: Acts as a Core Service wrapping the graphics backend. It handles batching, coordinate transformations via the `Viewport`, and sending draw calls to the GPU.

* **Backend Agnosticism**: Thanks to `SDL3_gpu`, the graphics module automatically detects and selects the most optimal GPU backend available on the host system (e.g., *Vulkan*, *Metal*, *DirectX11/DirectX12*, or *OpenGL*). This ensures maximum cross-platform performance without requiring conditional rendering code in the engine.

* **Custom Shaders**: The engine supports custom rendering pipelines via programmable shaders. While basic components (like `SpriteComponent`) use standard built-in shaders for texture rendering, the graphics API allows for custom shader binding. This enables advanced visual effects (e.g., lighting, outline highlights, or screen-space distortions) without needing to modify the high-level ECS logic.

* **Render Passes & Z-Indexing**: Rendering systems process entities based on a defined `zIndex` and specific tags (like `UIRenderTag` or `HiddenTag`). This ensures correct draw ordering, allowing UI to always render on top of the game world, and ensuring transparency is handled correctly across different visual layers.