#ifndef DELVE_GAME_OVER_SCENE_H
#define DELVE_GAME_OVER_SCENE_H

#include <scenes/GameplayScene.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

namespace delve
{

class GameOverScene : public dagger::Scene
{
public:
    void onInit(dagger::Application& app) override
    {
        auto uiFont = app.getAssets().load<dagger::Font>("ui", "assets/fonts/RobotoMono-Regular.ttf", 24);

        dagger::Entity* title = createEntity();
        auto& tTransform = title->addComponent<dagger::TransformComponent>();
        tTransform.setPosition(250.0f, 250.0f);
        
        auto& tText = title->addComponent<dagger::TextComponent>();
        tText.setFont(uiFont);
        tText.setColor(dagger::Colors::Red);
        tText.setText("YOU DIED\nPress ENTER to restart");
        tText.zIndex = 100;

        renderSystem_.registerEntity(title);
    }

    void onUpdate(float dt, dagger::Application& app) override
    {
        if (app.getInput().isKeyPressed(dagger::Key::Enter))
        {
            app.setScene(std::make_unique<GameplayScene>());
        }
    }
};

} // namespace delve

#endif // DELVE_GAME_OVER_SCENE_H
