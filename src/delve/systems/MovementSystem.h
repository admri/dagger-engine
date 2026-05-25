#ifndef DELVE_CORE_SYSTEMS_MOVEMENT_SYSTEM_H
#define DELVE_CORE_SYSTEMS_MOVEMENT_SYSTEM_H

namespace dagger
{
    struct TransformComponent;
    struct VelocityComponent;
}

namespace delve
{

class TileMap;

class MovementSystem
{
public:
    void update(float dt, dagger::TransformComponent& transform, dagger::VelocityComponent& velocity,
                const TileMap& tileMap, float width, float height);

    bool updateProjectile(float dt, dagger::TransformComponent& transform, dagger::VelocityComponent& velocity, 
                          const TileMap& tileMap, float width, float height);
};

} // namespace delve

#endif // DELVE_CORE_SYSTEMS_MOVEMENT_SYSTEM_H
