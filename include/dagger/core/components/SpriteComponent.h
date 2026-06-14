#ifndef DAGGER_CORE_COMPONENTS_SPRITE_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_SPRITE_COMPONENT_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>
#include <dagger/graphics/Texture.h>

namespace dagger
{

struct SpriteComponent : public Component
{
    TextureHandle texture;

    float width = 0.0f;
    float height = 0.0f;
    int zIndex = 0;

    float srcX = 0.0f;
    float srcY = 0.0f;
    float srcWidth = 0.0f;
    float srcHeight = 0.0f;
    
    bool hasSrcRect = false;
    bool flipX = false;
};

DAGGER_SERIALIZE_COMPONENT(SpriteComponent, width, height, zIndex, srcX, srcY, srcWidth, srcHeight, hasSrcRect, flipX)

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_SPRITE_COMPONENT_H
