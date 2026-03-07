#ifndef DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H

#include <dagger/core/Component.h>

namespace dagger
{

struct TransformComponent : Component
{
    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;
    float rotation = 0.0f;
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H
