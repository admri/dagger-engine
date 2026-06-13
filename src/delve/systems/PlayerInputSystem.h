#ifndef DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H
#define DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H

namespace dagger
{
    class Application;
    class Scene;
}

namespace delve
{

class PlayerInputSystem
{
public:
    void update(dagger::Application& app, dagger::Scene& scene);
};

} // namespace delve

#endif // DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H
