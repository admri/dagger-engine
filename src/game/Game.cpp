#include <game/Game.h>

#include <engine/core/SpriteObject.h>

Game::~Game()
{
    quit();
}

bool Game::onInit()
{
    auto myScene = std::make_unique<engine::Scene>();
    engine::TextureHandle texture = getAssets().load<engine::Texture>("player", "assets/player.png");
    
    auto player = std::make_unique<engine::SpriteObject>(texture);
    player->setPosition(50.0f, 50.0f);
    
    myScene->addGameObject(std::move(player));
    setScene(std::move(myScene));

    return true;
}

void Game::onUpdate(float dt)
{
    Input& input = getInput();

    if (input.isKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        quit();
    }
    if (input.isKeyPressed(SDL_SCANCODE_V))
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
