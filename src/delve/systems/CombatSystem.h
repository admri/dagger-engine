#ifndef DELVE_SYSTEMS_COMBAT_SYSTEM_H
#define DELVE_SYSTEMS_COMBAT_SYSTEM_H

namespace dagger
{
    class Application;
    class Scene;
    class Entity;
}

namespace delve
{

class CombatSystem
{
public:
    void update(float dt, dagger::Application& app, dagger::Scene& scene, dagger::Entity* player);
};

} // namespace delve

#endif // DELVE_SYSTEMS_COMBAT_SYSTEM_H
