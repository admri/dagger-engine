#include <game/Game.h>

#include <engine/core/GameEvent.h>
#include <engine/core/SpriteObject.h>
#include <engine/input/KeyCodes.h>
#include <engine/input/Input.h>

#include <iostream>

using namespace engine;

bool Game::onInit()
{
    auto myScene = std::make_unique<Scene>();
    TextureHandle texture = getAssets().load<Texture>("player", "assets/player.png");
    
    auto player = std::make_unique<SpriteObject>(texture);
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

void Game::onEvent(const engine::GameEvent &event)
{
}

void Game::onShutdown()
{
}
