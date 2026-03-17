#ifndef DAGGER_CORE_COMPONENTS_SPRITE_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_SPRITE_COMPONENT_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Component.h>
#include <dagger/graphics/Texture.h>

namespace dagger
{

struct SpriteComponent : public Component
{
    TextureHandle texture;
    int width;
    int height;
    int zIndex = 0;
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_SPRITE_COMPONENT_H
