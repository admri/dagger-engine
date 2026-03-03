#include <core/SpriteObject.h>

#include <assets/AssetManager.h>
#include <graphics/Renderer.h>
#include <graphics/Texture.h>

namespace engine
{

SpriteObject::SpriteObject(TextureHandle handle)
    : textureHandle_(handle)
{
}

void SpriteObject::setTexture(TextureHandle handle)
{
    textureHandle_ = handle;
}

void SpriteObject::setSourceRect(const SDL_FRect& srcRect)
{
    sourceRect_ = srcRect;
    hasSourceRect_ = true;
}

void SpriteObject::clearSourceRect()
{
    hasSourceRect_ = false;
}

void SpriteObject::render(Renderer& renderer, AssetManager& assets)
{
    Texture* texture = assets.resolve<Texture>(textureHandle_);
    if (!texture) return;

    const SDL_FRect* srcPtr = hasSourceRect_ ? &sourceRect_ : nullptr;

    renderer.drawTexture(*texture, x_, y_, srcPtr);
}

} // namespace engine
