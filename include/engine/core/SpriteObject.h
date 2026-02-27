#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include <core/GameObject.h>

#include <SDL3/SDL_rect.h>

class Texture;
class Renderer;

class SpriteObject : public GameObject
{
public:
    explicit SpriteObject(Texture* texture);

    void setTexture(Texture* texture);
    void setSourceRect(const SDL_FRect& srcRect);
    void clearSourceRect();

    void render(Renderer& renderer) override;

protected:
    Texture* texture_ = nullptr;
    SDL_FRect sourceRect_{};
    bool hasSourceRect_ = false;
};

#endif // SPRITE_OBJECT_H
