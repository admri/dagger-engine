#ifndef DELVE_SYSTEMS_COMBAT_SYSTEM_H
#define DELVE_SYSTEMS_COMBAT_SYSTEM_H

#include <vector>

namespace dagger
{
    class Application;
    class Scene;
    struct CollisionEvent;
}

namespace delve
{

class CombatSystem
{
public:
    void update(float dt, dagger::Application& app, dagger::Scene& scene, const std::vector<dagger::CollisionEvent>& collisions);
};

} // namespace delve

#endif // DELVE_SYSTEMS_COMBAT_SYSTEM_H
