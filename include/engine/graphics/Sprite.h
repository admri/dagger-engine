#ifndef SPRITE_H
#define SPRITE_H

#include <graphics/Texture.h>

class Sprite
{
public:
    Sprite(Texture* texture);

    void setTexture(Texture* texture);
    void setPosition(float x, float y);
    void setSourceRect(const SDL_FRect* srcRect);
    void clearSourceRect();

    void render(Renderer& renderer) const;

private:
    Texture* texture_ = nullptr;
    float x_ = 0.0f;
    float y_ = 0.0f;

    SDL_FRect sourceRect_;
    bool hasSourceRect_ = false;
};

#endif // SPRITE_H
