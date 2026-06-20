#include <graphics/Renderer.h>

#include <core/Window.h>
#include <graphics/Texture.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

namespace dagger
{

void SDLRendererDeleter::operator()(SDL_Renderer *renderer) const
{
    if (renderer) SDL_DestroyRenderer(renderer);
}

bool Renderer::create(const Window& window)
{
    SDL_Window* sdlWindow = window.getSDLWindow();
    if (!sdlWindow)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window not created yet.");
        return false;
    }

    // Create GPU Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, "gpu");
    if (renderer)
    {
        SDL_GPUDevice* device = SDL_GetGPURendererDevice(renderer);
        if (device)
        {
            const char* backendName = SDL_GetGPUDeviceDriver(device);
            SDL_Log("Using graphics API: %s", backendName);
        }
        hasShadersEnabled = true;
    }
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to create 'gpu' renderer, falling back to default: %s", SDL_GetError());
        renderer = SDL_CreateRenderer(sdlWindow, NULL);
    }

    // Fallback: Create default SDL Renderer
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Renderer: %s", SDL_GetError());
        return false;
    }

    sdlRenderer_.reset(renderer);
    return true;
}

void Renderer::destroy()
{
    if (!sdlRenderer_)
    {
        return;
    }

    sdlRenderer_.reset();
}

void Renderer::clear(Color clearColor)
{
    if (!sdlRenderer_)
    {
        return;
    }

    SDL_SetRenderDrawColor(sdlRenderer_.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(sdlRenderer_.get());
}

void Renderer::present()
{
    if (!sdlRenderer_)
    {
        return;
    }

    SDL_RenderPresent(sdlRenderer_.get());
}

void Renderer::drawTexture(const Texture& texture, const Rect& dstRect, const Rect* srcRect, bool flipX, Color tint)
{
    SDL_Texture* sdlTex = texture.getSDLTexture();

    if (!sdlRenderer_ || !texture.getSDLTexture())
    {
        return;
    }

    if (tint != Colors::White)
    {
        SDL_SetTextureColorMod(sdlTex, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaMod(sdlTex, tint.a);
    }

    SDL_FRect sdlDstRect = { dstRect.x, dstRect.y, dstRect.w, dstRect.h };

    SDL_FRect sdlSrcRect;
    SDL_FRect* pSrcRect = nullptr;
    if (srcRect)
    {
        sdlSrcRect = { srcRect->x, srcRect->y, srcRect->w, srcRect->h };
        pSrcRect = &sdlSrcRect;
    }

    SDL_FlipMode flipMode = flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    if (!SDL_RenderTextureRotated(sdlRenderer_.get(), texture.getSDLTexture(), pSrcRect, &sdlDstRect, 0.0, nullptr, flipMode))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render texture: %s", SDL_GetError());
    }

    if (tint != Colors::White)
    {
        SDL_SetTextureColorMod(sdlTex, 255, 255, 255);
        SDL_SetTextureAlphaMod(sdlTex, 255);
    }
}

void Renderer::drawRect(const Rect& rect, Color color)
{
    if (!sdlRenderer_) return;

    // Store old color
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(sdlRenderer_.get(), &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    // Render rect with new color
    SDL_SetRenderDrawColor(sdlRenderer_.get(), color.r, color.g, color.b, color.a);
    SDL_FRect sdlRect = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderRect(sdlRenderer_.get(), &sdlRect);

    // Revert to old color
    SDL_SetRenderDrawColor(sdlRenderer_.get(), oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

void Renderer::fillRect(const Rect& rect, Color color)
{
    if (!sdlRenderer_) return;

    SDL_Color oldColor;
    SDL_GetRenderDrawColor(sdlRenderer_.get(), &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    SDL_SetRenderDrawColor(sdlRenderer_.get(), color.r, color.g, color.b, color.a);
    SDL_FRect sdlRect = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderFillRect(sdlRenderer_.get(), &sdlRect);

    SDL_SetRenderDrawColor(sdlRenderer_.get(), oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

void Renderer::toggleVSync()
{
    if (!sdlRenderer_) return;

    int currentVsync = 0;
    SDL_GetRenderVSync(sdlRenderer_.get(), &currentVsync);
    
    int targetVsync = (currentVsync == SDL_RENDERER_VSYNC_DISABLED) ? 1 : SDL_RENDERER_VSYNC_DISABLED;

    SDL_Log("Toggling VSync to: %s", targetVsync == 1 ? "ON" : "OFF");

    if (!SDL_SetRenderVSync(sdlRenderer_.get(), targetVsync))
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Failed to toggle VSync: %s", SDL_GetError());
    }
}

int Renderer::getWidth() const
{
    int w, h;
    SDL_GetRenderOutputSize(sdlRenderer_.get(), &w, &h);
    return w;
}

int Renderer::getHeight() const
{
    int w, h;
    SDL_GetRenderOutputSize(sdlRenderer_.get(), &w, &h);
    return h;
}

void Renderer::setRenderTarget(Texture* target)
{
    if (!sdlRenderer_) return;
    
    SDL_Texture* sdlTex = target ? target->getSDLTexture() : nullptr;
    if (!SDL_SetRenderTarget(sdlRenderer_.get(), sdlTex))
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to set render target: %s", SDL_GetError());
    }
}

Renderer::ShaderData Renderer::loadFragmentShader(const std::string& filepath, uint32_t uniformBufferSize)
{
    ShaderData result;
    if (!hasShadersEnabled || !sdlRenderer_) return result;

    // Retrieve the graphics API
    SDL_GPUDevice* device = SDL_GetGPURendererDevice(sdlRenderer_.get());
    if (!device)  return result;

    SDL_GPUShaderFormat supportedFormats = SDL_GetGPUShaderFormats(device);
    
    std::string finalPath = filepath;
    SDL_GPUShaderFormat shaderFormat;

    // Match the supported shader format
    if (supportedFormats & SDL_GPU_SHADERFORMAT_DXIL) 
    {
        // DirectX
        finalPath += ".dxil";
        shaderFormat = SDL_GPU_SHADERFORMAT_DXIL;
    }
    else if (supportedFormats & SDL_GPU_SHADERFORMAT_SPIRV) 
    {
        // Vulkan
        finalPath += ".spv";
        shaderFormat = SDL_GPU_SHADERFORMAT_SPIRV;
    }
    else if (supportedFormats & SDL_GPU_SHADERFORMAT_MSL) 
    {
        // Metal
        finalPath += ".msl";
        shaderFormat = SDL_GPU_SHADERFORMAT_MSL;
    } 
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No supported shader format found for this GPU backend.");
        return result;
    }

    // Load the binary file (.spv / .dxil / .msl)
    size_t codeSize;
    void* code = SDL_LoadFile(finalPath.c_str(), &codeSize);
    if (!code) 
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load shader file: %s", finalPath.c_str());
        return result;
    }

    // Create the shader
    SDL_GPUShaderCreateInfo shaderInfo = {};
    shaderInfo.code = static_cast<const Uint8*>(code);
    shaderInfo.code_size = codeSize;
    shaderInfo.format = shaderFormat;
    shaderInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    shaderInfo.num_samplers = 1;
    shaderInfo.num_storage_textures = 0;
    shaderInfo.num_storage_buffers = 0;
    shaderInfo.num_uniform_buffers = uniformBufferSize > 0 ? 1 : 0;
    
    // SPIRV-Cross renames the entrypoint to "main0" when compiling into MSL (Metal)
    shaderInfo.entrypoint = (shaderFormat == SDL_GPU_SHADERFORMAT_MSL) ? "main0" : "main";

    result.shader = SDL_CreateGPUShader(device, &shaderInfo);
    SDL_free(code);

    if (result.shader)
    {
        SDL_GPURenderStateCreateInfo stateInfo = {};
        stateInfo.fragment_shader = result.shader;
        result.state = SDL_CreateGPURenderState(sdlRenderer_.get(), &stateInfo);

        if (!result.state)
        {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to create GPU Render State: %s", SDL_GetError());
        }
    }

    return result;
}

void Renderer::setShader(const ShaderData* shader)
{
    if (!hasShadersEnabled || !sdlRenderer_) return;
    SDL_SetGPURenderState(sdlRenderer_.get(), shader ? shader->state : nullptr);
}

void Renderer::setShaderUniforms(ShaderData* shader, const void* data, size_t size)
{
    if (!hasShadersEnabled || !shader || !shader->state) return;
    SDL_SetGPURenderStateFragmentUniforms(shader->state, 0, data, size);
}

void Renderer::destroyShader(ShaderData& shader)
{
    if (!hasShadersEnabled || !sdlRenderer_) return;
    
    if (shader.state) 
    {
        SDL_DestroyGPURenderState(shader.state);
        shader.state = nullptr;
    }
    if (shader.shader) 
    {
        SDL_GPUDevice* device = SDL_GetGPURendererDevice(sdlRenderer_.get());
        SDL_ReleaseGPUShader(device, shader.shader);
        shader.shader = nullptr;
    }
}

} // namespace dagger
