#ifndef DAGGER_CORE_SYSTEMS_MOVEMENT_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_MOVEMENT_SYSTEM_H

namespace dagger
{

struct TransformComponent;
struct VelocityComponent;

class MovementSystem
{
public:
    void update(float dt, TransformComponent& transform, VelocityComponent& velocity);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_MOVEMENT_SYSTEM_H
