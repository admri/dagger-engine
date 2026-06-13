#ifndef DELVE_SYSTEMS_SHOOTING_SYSTEM_H
#define DELVE_SYSTEMS_SHOOTING_SYSTEM_H

namespace dagger
{
    class Application;
    class Scene;
}

namespace delve
{

class ShootingSystem {
public:
    void update(float dt, dagger::Application& app, dagger::Scene& scene);
};

} // namespace delve

#endif // DELVE_SYSTEMS_SHOOTING_SYSTEM_H
