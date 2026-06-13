#ifndef DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H
#define DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H

namespace dagger
{
    class Application;
    class Scene;
    class Entity;
}

namespace delve
{

class PlayerInputSystem
{
public:
    void update(dagger::Application& app, dagger::Scene& scene, dagger::Entity* camera);
};

} // namespace delve

#endif // DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H
