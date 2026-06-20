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
    friend class Renderer;

public:
    Texture() = default;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(Texture&& other) noexcept = default;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    bool createFromSurface(const Renderer& renderer, SDL_Surface& surface);
    bool createRenderTarget(Renderer& renderer, int width, int height);
    bool load(const Renderer& renderer, const std::string& path);
    void destroy();

    inline float getWidth() const { return width_; }
    inline float getHeight() const { return height_; }
    inline bool isValid() const { return sdlTexture_ != nullptr; }

private:
    inline SDL_Texture* getSDLTexture() const { return sdlTexture_.get(); }

    std::unique_ptr<SDL_Texture, SDLTextureDeleter> sdlTexture_;
    float width_ = 0.0f;
    float height_ = 0.0f;
};

} // namespace dagger

#endif // DAGGER_GRAPHICS_TEXTURE_H
