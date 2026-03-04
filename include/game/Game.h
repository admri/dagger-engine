#ifndef GAME_H
#define GAME_H

#include <engine/core/Application.h>

class Game : public engine::Application
{
protected:
    bool onInit() override;
    void onUpdate(float dt) override;
    void onRender() override;
    void onEvent(const engine::GameEvent& event) override;
    void onShutdown() override;
};

#endif // GAME_H
