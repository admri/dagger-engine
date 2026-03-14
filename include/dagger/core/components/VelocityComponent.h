#ifndef DAGGER_CORE_COMPONENTS_VELOCITY_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_VELOCITY_COMPONENT_H

namespace dagger
{

struct VelocityComponent : public Component
{
    float vx = 0.0f;
    float vy = 0.0f;
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_VELOCITY_COMPONENT_H
