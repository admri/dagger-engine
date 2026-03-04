#ifndef DAGGER_CORE_SPRITE_OBJECT_H
#define DAGGER_CORE_SPRITE_OBJECT_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/GameObject.h>

#include <SDL3/SDL_rect.h>

namespace dagger
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

} // namespace dagger

#endif // DAGGER_CORE_SPRITE_OBJECT_H
