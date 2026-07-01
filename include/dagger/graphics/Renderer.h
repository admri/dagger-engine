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

/// @brief Core hardware service wrapping the graphics backend.
/// Handles draw calls, texture rendering, and shader pipeline management.
class Renderer
{
    friend class Texture;
    friend class Application;

public:
    /// @brief Initializes the rendering context for the given window.
    bool create(const Window& window);
    void destroy();

    /// @brief Clears the backbuffer with a solid color. Called at the start of the frame.
    void clear(Color clearColor = Colors::Black);
    
    /// @brief Swaps the backbuffer to the screen. Called at the end of the frame.
    void present();

    /// @brief Renders a texture to the screen.
    /// @param texture The loaded texture to draw.
    /// @param dstRect Position and scale on the screen.
    /// @param srcRect Optional crop area of the texture (used for spritesheets).
    /// @param flipX Whether to horizontally mirror the texture.
    /// @param tint Color multiplier to apply to the texture.
    void drawTexture(const Texture& texture, const Rect& dstRect, const Rect* srcRect = nullptr, bool flipX = false, Color tint = Colors::White);
    
    void drawRect(const Rect& rect, Color color);
    void fillRect(const Rect& rect, Color color);

    void toggleVSync();
    int getWidth() const;
    int getHeight() const;

    /// @brief Redirects all subsequent draw calls to a specific texture instead of the screen.
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
