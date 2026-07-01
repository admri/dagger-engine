# **Dagger** Engine & **Delve** - User Manual

This document is divided into three parts. The first part is a **Getting Started Guide** for C++ programmers who want to bootstrap a new game using the *Dagger* engine. The second part is a **Content Creator Guide**, explaining how to use the engine's data-driven pipeline to build levels and define entities without writing code. The third part is a **Player Guide** for *Delve*, the sample game built on this engine.

## PART 1: Getting Started (Gameplay Programmer)

To build a game using the **Dagger** engine, you need to write a small amount of C++ bootstrapping code. The engine handles the main loop, window management, and hardware abstraction, but you must define your game class and your scenes by overriding their core lifecycle methods.

### 1. The Entry Point

Every game starts by instantiating your custom application class and calling its run method inside `main(int argc, char* argv[])` (`main()` without passing command-line arguments will not work for internal engine reasons). You can also pass an app configuration such as various window properties, renderer settings or even game logic settings into the game.

```cpp
#include <dagger/main.h> // Necessary include to correctly define the application entry point
#include <dagger/core/AppConfig.h>

#include "MyGame.h"

int main(int argc, char* argv[])
{
    dagger::AppConfig config{
        .title = "Delve - Dungeon Crawler",
        .width = 1280,
        .height = 720,
        .flags = dagger::WindowFlags::Resizable | dagger::WindowFlags::HighPixelDensity,
        .vsync = true,
        .tps = 60 // Ticks per second
    };

    MyGame game;
    game.run(config);
    return 0;
}
```

### 2. Bootstrapping the Application

Your main class must inherit from `dagger::Application`. This class acts as the central hub. The engine provides several virtual lifecycle methods (`onInit`, `onUpdate`, `onRender`, `onEvent`, `onShutdown`) that you can override to control the global flow of your game.

```cpp
#include <dagger/core/Application.h>

class MyGame : public dagger::Application
{
public:
    bool onInit() override
    {
        // Register custom game components for the Prefab Factory
        registerGameComponents();
        
        // Load the initial scene (e.g., Main Menu)
        getSceneManager().replace(std::make_unique<MainMenuScene>());
        
        return true;
    }

    void onUpdate(float dt) override
    {
        // Global update logic independent of the current scene

        // Handle global input (e.g., pausing, window management)
        if (getInput().isKeyPressed(dagger::Key::Escape)) quit();
        if (getInput().isKeyPressed(dagger::Key::V)) getRenderer().toggleVSync();
    }
    
    void onRender() override
    {
        // Global rendering (e.g., persistent debug UI overlays)
    }
};
```

### 3. Defining Components

Components are Plain Old Data (POD) structures that inherit from `dagger::Component`. To make a component loadable from JSON, you must use the provided serialization macro at the bottom of the struct.

```cpp
#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

struct HealthComponent : public dagger::Component
{
    int currentHP = 100;
    int maxHP = 100;
};

// This macro automatically generates JSON parsing rules for the primitive types
DAGGER_SERIALIZE_COMPONENT(HealthComponent, currentHP, maxHP)
```

### 4. Component Registration

Before the engine can load entity blueprints (Prefabs) from JSON, you must tell the `PrefabFactory` how to deserialize your game-specific components.

```cpp
void MyGame::registerGameComponents()
{
    // Register basic POD components using built-in macros
    dagger::PrefabFactory::registerComponent<HealthComponent>("HealthComponent");
    
    // Register complex components, e.g. those that require Asset resolution (e.g., Audio Handles)
    dagger::PrefabFactory::registerComponent("WeaponComponent", 
        [](dagger::Entity* e, const auto& json, dagger::AssetManager& assets) {
            auto& weapon = e->addComponent<WeaponComponent>();
            weapon = json.get<WeaponComponent>();
            
            if (json.contains("shootSound")) {
                weapon.shootSound = assets.getSound(json["shootSound"].get<std::string>());
            }
        });
}
```

### 5. Creating Systems

Systems contain the game logic. They query the active `Scene` for entities that match a specific signature (a combination of components) and update them.

```cpp
#include <dagger/core/Scene.h>
#include <dagger/core/Application.h>

class MovementSystem
{
public:
    void update(float dt, dagger::Scene& scene)
    {
        // Query only entities that have both Transform and Velocity
        scene.forEachEntityWith<TransformComponent, VelocityComponent>(
            [&](dagger::Entity* entity, auto& transform, auto& velocity) {
                transform.x += velocity.vx * dt;
                transform.y += velocity.vy * dt;
            });
    }
};
```

### 6. Creating Scenes & Managing Systems

Gameplay happens inside `Scene` objects. A custom scene inherits from `dagger::Scene` and overrides its own lifecycle methods (`onInit`, `onUpdate`, `onFixedUpdate`, `onInput`, `onRender`).

The `Scene` typically holds instances of its ECS systems and calls their update methods manually. This gives you absolute control over the **Execution Order** (e.g., ensuring movement happens before collision).

```cpp
#include <dagger/core/Scene.h>

#include <systems/PlayerInputSystem.h>
#include <systems/ShootingSystem.h>

class LevelScene : public dagger::Scene
{
private:
    // The scene owns its logic systems
    delve::PlayerInputSystem inputSystem_;
    delve::ShootingSystem shootingSystem_;

public:
    bool onInit() override
    {
        // Load the level data and populate entities via LevelLoader
        dagger::LevelLoader::load("assets/levels/level1.json", *this, app->getAssets());
        return true;
    }

    void onFixedUpdate(float dt) override
    {
        // Manually update systems in the exact order required
        inputSystem_.update(dt, *app, *this);
        shootingSystem_.update(dt, *app, *this);
    }
    
    void onRender() override
    {
        // Delegate rendering to visual systems
    }
};
```

#### The Game Loop: Update vs. Fixed Update
The engine separates standard frame updates from fixed timestep updates based on the `tps` (Ticks Per Second - default 60) setting in your `AppConfig`. 

* `onUpdate(float dt)`: Called every frame. The `dt` fluctuates based on framerate. Use this for input handling, camera movement, or rendering interpolation.
* `onFixedUpdate(float dt)`: Called at a strict, constant interval (e.g., 60 times a second). The `dt` is expected to be always the same. Use this for movement, physics, collision detection, and AI to ensure deterministic behavior regardless of the player's framerate.

## PART 2: Content Creator Guide (Dagger Engine)

Once the C++ bootstrapping is done, the rest of the game content is strictly data-driven. All data should be stored in the `assets` directory in the project root which `dagger::AssetManager` is able to access. All game content is defined using JSON files. The content pipeline consists of three main parts:

### 1. The Asset Manifest (`manifest.json`)

Before a level starts, the engine needs to know which files to load into memory to prevent stuttering during gameplay.

```json
{
  "textures": [
    { "id": "player_tex", "path": "textures/player.png" },
    { "id": "dungeon_tiles", "path": "textures/tileset.png" }
  ],
  "fonts": [],
  "music": [],
  "sounds": [
    { "id": "fireball_shoot", "path": "audio/fireball.wav" }
  ],
  "prefabs": [
    { "id": "fireball", "path": "prefabs/fireball.json" },
    { "id": "skeleton", "path": "prefabs/skeleton.json" }
  ]
}
```

### 2. Prefabs (Entity Blueprints)

Prefabs define the components that make up an entity. By creating a new JSON file, you can create entirely new enemies, weapons, or props without touching the source code.

Example: `assets/prefabs/weapon_staff.json`

```json
{
    "components": {
        "SpriteComponent": {
            "texture": "weapon_staff",
            "width": 32,
            "height": 32,
            "zIndex": 5
        },
        "WeaponComponent": {
            "fireRate": 0.25,
            "projectileSpeed": 600.0,
            "damage": 10,
            "shootSound": "fireball_shoot"
        }
    }
}
```

### 3. Level Layout (level.json)

The level file defines the required manifest alongside its entities and their positions (more entities can be later added programatically).

```json
{
  "name": "Dungeon Floor 1",
  "required_manifest": "manifests/manifest.json",
  "entities": [
    {
      "prefab": "skeleton",
      "x": 128.0,
      "y": 64.0
    },
    {
      "prefab": "player",
      "x": 64.0,
      "y": 64.0
    }
  ]
}
```

## PART 3: Player Guide (Delve)

**Delve** is a top-down dungeon crawler built to showcase the capabilities of the *Dagger* engine.

### Objective

Navigate through the dark dungeon, manage your health, and eliminate enemies using your ranged weapons.

### Controls

* W, A, S, D: Move the character.
* Mouse: Aim your weapon.
* Left Mouse Button: Fire weapon.
* P: Pause the game.
* V: Toggle V-Sync.
* ESC: Quit game.

### User Interface

* Health Bar: Displayed in the top left corner.

* FPS Counter: Displayed in the top right corner to monitor engine performance.