#ifndef DELVE_GAME_H
#define DELVE_GAME_H

#include <dagger/core/Application.h>

namespace delve
{

class Game : public dagger::Application
{
protected:
    bool onInit() override;
    void onUpdate(float dt) override;
    void onRender() override;
    void onEvent(const dagger::GameEvent& event) override;
    void onShutdown() override;

private:
    void registerGameComponents();
};

} // namespace delve

#endif // DELVE_GAME_H
