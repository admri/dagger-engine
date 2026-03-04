#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include <engine/assets/ResourceHandle.h>
#include <engine/core/GameObject.h>

#include <SDL3/SDL_rect.h>

namespace engine
{

class Texture;
class Renderer;
class AssetManager;

class SpriteObject : public GameObject
{
public:
    explicit SpriteObject(TextureHandle handle);

    void setTexture(TextureHandle handle);
    void setSourceRect(const SDL_FRect& srcRect);
    void clearSourceRect();

    void render(Renderer& renderer, AssetManager& assets) override;

protected:
    TextureHandle textureHandle_;
    SDL_FRect sourceRect_{};
    bool hasSourceRect_ = false;
};

} // namespace engine

#endif // SPRITE_OBJECT_H
