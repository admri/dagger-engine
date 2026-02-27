#include <core/SpriteObject.h>

#include <graphics/Renderer.h>
#include <graphics/Texture.h>

SpriteObject::SpriteObject(Texture* texture)
    : texture_(texture)
{
}

void SpriteObject::setTexture(Texture* texture)
{
    texture_ = texture;
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

void SpriteObject::render(Renderer& renderer)
{
    if (!texture_)
    {
        return;
    }

    const SDL_FRect* srcPtr = hasSourceRect_ ? &sourceRect_ : nullptr;

    renderer.drawTexture(*texture_, x_, y_, srcPtr);
}
