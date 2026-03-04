#ifndef DAGGER_GRAPHICS_TEXTURE_H
#define DAGGER_GRAPHICS_TEXTURE_H

#include <memory>
#include <string>

struct SDL_Surface;
struct SDL_Texture;

namespace dagger
{

class Renderer;

struct SDLTextureDeleter
{
    void operator()(SDL_Texture* texture) const;
};

class Texture
{
public:
    bool createFromSurface(const Renderer& renderer, SDL_Surface& surface);
    bool load(const Renderer& renderer, const std::string& path);
    void destroy();

    SDL_Texture* getSDLTexture() const { return sdlTexture_.get(); }
    float getWidth() const { return width_; }
    float getHeight() const { return height_; }

private:
    std::unique_ptr<SDL_Texture, SDLTextureDeleter> sdlTexture_;
    float width_ = 0.0f;
    float height_ = 0.0f;
};

} // namespace dagger

#endif // DAGGER_GRAPHICS_TEXTURE_H
