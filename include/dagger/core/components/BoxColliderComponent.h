#ifndef DAGGER_CORE_COMPONENTS_BOX_COLLIDER_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_BOX_COLLIDER_COMPONENT_H

#include <dagger/core/Component.h>

namespace dagger
{

struct BoxColliderComponent : public Component
{
    float width = 0.0f;
    float height = 0.0f;
    
    // Hitbox offset from actual entity coordinates
    float offsetX = 0.0f; 
    float offsetY = 0.0f;
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_BOX_COLLIDER_COMPONENT_H
