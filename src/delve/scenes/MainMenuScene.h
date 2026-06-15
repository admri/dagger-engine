#ifndef DELVE_SCENES_MAIN_MENU_SCENE_H
#define DELVE_SCENES_MAIN_MENU_SCENE_H

#include <scenes/GameplayScene.h>

#include <dagger/core/Scene.h>
#include <dagger/core/Application.h>
#include <dagger/assets/AssetManager.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/systems/UIRenderSystem.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

namespace delve
{

class MainMenuScene : public dagger::Scene
{
public:
    void onInit(dagger::Application& app) override
    {
        auto uiFont = app.getAssets().load<dagger::Font>("ui", "fonts/RobotoMono-Regular.ttf", 24);

        float centerX = (app.getWindowWidth() / 2.0f) - 120.0f;
        float startY = 150.0f;

        createText("DELVE", centerX + 50.0f, startY, uiFont, dagger::Colors::Red);
        createText("Press ENTER to start", centerX, startY + 50.0f, uiFont);
        
        createText("Controls: WASD, LMB", centerX - 20.0f, startY + 150.0f, uiFont);
        createText("Press V to toggle V-Sync", centerX - 40.0f, startY + 200.0f, uiFont);
        createText("Press Esc to exit game", centerX - 30.0f, startY + 250.0f, uiFont);
    }

    void onUpdate(float dt, dagger::Application& app) override
    {
        if (app.getInput().isKeyPressed(dagger::Key::Enter))
        {
            app.getSceneManager().replace(std::make_unique<GameplayScene>());
        }
    }

    void onRender(dagger::Renderer& renderer, dagger::AssetManager& assets, float alpha = 0.0f) override
    {
        uiRenderSystem_.render(renderer, assets, *this, alpha);
    }

private:
    dagger::UIRenderSystem uiRenderSystem_;
};

} // namespace delve

#endif // DELVE_SCENES_MAIN_MENU_SCENE_H
