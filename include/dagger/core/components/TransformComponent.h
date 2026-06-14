#ifndef DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H

#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

namespace dagger
{

struct TransformComponent : public Component
{
    float x = 0.0f;
    float y = 0.0f;
    
    float prevX = 0.0f; 
    float prevY = 0.0f; 
    
    void setPosition(float newX, float newY)
    {
        x = newX; y = newY;
        prevX = newX; prevY = newY;
    }
};

DAGGER_SERIALIZE_COMPONENT(TransformComponent, x, y)

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H
