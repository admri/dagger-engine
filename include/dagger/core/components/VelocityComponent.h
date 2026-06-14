#ifndef DAGGER_CORE_COMPONENTS_VELOCITY_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_VELOCITY_COMPONENT_H

#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

namespace dagger
{

struct VelocityComponent : public Component
{
    float vx = 0.0f;
    float vy = 0.0f;
};

DAGGER_SERIALIZE_COMPONENT(VelocityComponent, vx, vy)

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_VELOCITY_COMPONENT_H
