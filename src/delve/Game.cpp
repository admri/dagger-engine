#include <Game.h>

#include <scenes/GameplayScene.h>

#include <dagger/core/GameEvent.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/input/KeyCodes.h>
#include <dagger/input/Input.h>

namespace delve
{
    
using namespace dagger;

bool Game::onInit()
{
    auto gameplayScene = std::make_unique<GameplayScene>();
    setScene(std::move(gameplayScene));
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
