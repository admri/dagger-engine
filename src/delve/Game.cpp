#include <Game.h>

#include <scenes/MainMenuScene.h>

#include <dagger/core/GameEvent.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/input/KeyCodes.h>
#include <dagger/input/Input.h>

namespace delve
{
    
using namespace dagger;

bool Game::onInit()
{
    auto mainMenuScene = std::make_unique<MainMenuScene>();
    setScene(std::move(mainMenuScene));
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
