#include <Game.h>

#include <components/EnemyAIComponent.h>
#include <components/FOVComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/ProjectileComponent.h>
#include <components/Tags.h>
#include <components/WeaponComponent.h>
#include <scenes/MainMenuScene.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/PrefabFactory.h>
#include <dagger/audio/AudioResource.h>
#include <dagger/core/GameEvent.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/input/KeyCodes.h>
#include <dagger/input/Input.h>

namespace delve
{
    
using namespace dagger;

void Game::registerGameComponents()
{
    PrefabFactory::registerComponent<EnemyAIComponent>("EnemyAIComponent");
    PrefabFactory::registerComponent<FOVComponent>("FOVComponent");
    PrefabFactory::registerComponent<HealthComponent>("HealthComponent");
    PrefabFactory::registerComponent<PlayerInputComponent>("PlayerInputComponent");
    PrefabFactory::registerComponent<ProjectileComponent>("ProjectileComponent");

    PrefabFactory::registerComponent("WeaponComponent", [](dagger::Entity* e, const auto& json, dagger::AssetManager& assets)
    {
        auto& weapon = e->addComponent<WeaponComponent>();

        weapon = json.get<WeaponComponent>();

        if (json.contains("shootSound"))
        {
            std::string soundName = json["shootSound"].get<std::string>();
            weapon.shootSound = assets.get<dagger::Sound>(soundName); 
        }
    });

    PrefabFactory::registerTag<FPSCounterTag>("FPSCounterTag");
    PrefabFactory::registerTag<HealthDisplayTag>("HealthDisplayTag");
    PrefabFactory::registerTag<WorldObserverTag>("WorldObserverTag");
}

bool Game::onInit()
{
    registerGameComponents();
    auto mainMenuScene = std::make_unique<MainMenuScene>();
    getSceneManager().replace(std::move(mainMenuScene));
    return true;
}

void Game::onUpdate(float dt)
{
    Input& input = getInput();

    if (input.isKeyPressed(Key::Escape))
    {
        quit();
    }
    if (input.isKeyPressed(Key::V))
    {
        getRenderer().toggleVSync();
    }
}

void Game::onRender()
{
}

void Game::onEvent(const GameEvent& event)
{
}

void Game::onShutdown()
{
}

} // namespace delve
