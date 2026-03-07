#ifndef DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H

#include <dagger/core/Component.h>

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

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_TRANSFORM_COMPONENT_H
