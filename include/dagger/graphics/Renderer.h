#ifndef DAGGER_GRAPHICS_RENDERER_H
#define DAGGER_GRAPHICS_RENDERER_H

#include <dagger/graphics/Color.h>
#include <dagger/graphics/Rect.h>

#include <memory>

struct SDL_Renderer;
struct SDL_GPUShader;
struct SDL_GPURenderState;

namespace dagger
{

class Texture;
class Window;

struct SDLRendererDeleter
{
    void operator()(SDL_Renderer* renderer) const;
};

class Renderer
{
    friend class Texture;
    friend class Application;

public:
    bool create(const Window& window);
    void destroy();

    void clear(Color clearColor = Colors::Black);
    void present();

    void drawTexture(const Texture& texture, const Rect& dstRect, const Rect* srcRect = nullptr, bool flipX = false, Color tint = Colors::White);
    void drawRect(const Rect& rect, Color color);
    void fillRect(const Rect& rect, Color color);

    void toggleVSync();
    int getWidth() const;
    int getHeight() const;

    void setRenderTarget(Texture* target);

    // Shader management
    struct ShaderData
    {
        SDL_GPUShader* shader = nullptr;
        SDL_GPURenderState* state = nullptr;
    };

    ShaderData loadFragmentShader(const std::string& filepath, uint32_t uniformBufferSize);
    void setShader(const ShaderData* shader);
    void setShaderUniforms(ShaderData* shader, const void* data, size_t size);
    void destroyShader(ShaderData& shader);
    bool areShadersSupported() const { return hasShadersEnabled; }

private:
    SDL_Renderer* get() const { return sdlRenderer_.get(); }

    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdlRenderer_;
    bool hasShadersEnabled = false;
};

} // namespace dagger

#endif // DAGGER_GRAPHICS_RENDERER_H
