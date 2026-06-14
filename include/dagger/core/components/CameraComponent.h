#ifndef DAGGER_CORE_COMPONENTS_CAMERA_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_CAMERA_COMPONENT_H

#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

namespace dagger
{

struct CameraComponent : public Component
{
    float zoom = 1.0f;
    bool isActive = true;
};

DAGGER_SERIALIZE_COMPONENT(CameraComponent, zoom, isActive)

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_CAMERA_COMPONENT_H
