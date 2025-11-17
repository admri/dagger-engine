#include <graphics/Sprite.h>

Sprite::Sprite(Texture* texture)
    : texture_(texture)
{
}

void Sprite::setTexture(Texture* texture)
{
    texture_ = texture;
}

void Sprite::setPosition(float x, float y)
{
    x_ = x;
    y_ = y;
}

void Sprite::setSourceRect(const SDL_FRect* srcRect)
{
    if (!srcRect)
    {
        return;
    }

    sourceRect_ = *srcRect;
    hasSourceRect_ = true;
}

void Sprite::clearSourceRect()
{
    hasSourceRect_ = false;
}

void Sprite::render(Renderer& renderer) const
{
    if (!texture_)
    {
        return;
    }

    if (hasSourceRect_)
    {
        renderer.drawTexture(*texture_, &sourceRect_, nullptr);
    }
    else
    {
        renderer.drawTexture(*texture_, nullptr, nullptr);
    }
}
