#ifndef DELVE_CORE_SYSTEMS_MOVEMENT_SYSTEM_H
#define DELVE_CORE_SYSTEMS_MOVEMENT_SYSTEM_H

namespace dagger
{
    struct TransformComponent;
    struct VelocityComponent;
    struct Scene;
}

namespace delve
{

class TileMap;

class MovementSystem
{
public:
    bool update(float dt, dagger::TransformComponent& transform, dagger::VelocityComponent& velocity,
                const TileMap& map, float w, float h, float offsetX, float offsetY);
    void updateAll(float dt, dagger::Scene& scene, TileMap& map);
};

} // namespace delve

#endif // DELVE_CORE_SYSTEMS_MOVEMENT_SYSTEM_H
