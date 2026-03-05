#include <Game.h>

#include <entities/Player.h>

#include <dagger/core/GameEvent.h>
#include <dagger/core/SpriteObject.h>
#include <dagger/input/KeyCodes.h>
#include <dagger/input/Input.h>

namespace delve
{
    
using namespace dagger;

bool Game::onInit()
{
    auto myScene = std::make_unique<Scene>();
    TextureHandle texture = getAssets().load<Texture>("player", "assets/player.png");
    
    auto player = std::make_unique<Player>(texture);
    player->setPosition(50.0f, 50.0f);
    
    myScene->addGameObject(std::move(player));
    setScene(std::move(myScene));

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
