#include <core/systems/RenderSystem.h>

#include <assets/AssetManager.h>
#include <core/Entity.h>
#include <core/Scene.h>
#include <dagger/core/components/CameraComponent.h>
#include <core/components/SpriteComponent.h>
#include <core/components/TextComponent.h>
#include <core/components/TransformComponent.h>
#include <graphics/Rect.h>
#include <graphics/Renderer.h>
#include <graphics/Texture.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <vector>

namespace dagger
{

namespace
{

SDL_Color toSDL(Color c)
{
    return {c.r, c.g, c.b, c.a};
}

} // namespace

struct RenderNode
{
    TransformComponent* transform;
    SpriteComponent* sprite = nullptr;
    TextComponent* text = nullptr;

    inline int getZIndex() const
    {
        if (sprite) return sprite->zIndex;
        if (text) return text->zIndex;
        return 0;
    }
};

bool rebuildTexture(TextComponent* text, Renderer& renderer, AssetManager& assets)
{
    if (text->text.empty()) 
    {
        text->cachedTexture.destroy();
        text->isDirty = false;
        return true;
    }

    Font* font = assets.resolve<Font>(text->font);
    if (!font) return false;

    SDL_Surface* surface = TTF_RenderText_Blended(
        font->get(),
        text->text.c_str(),
        text->text.length(),
        toSDL(text->color)
    );
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text surface: %s", SDL_GetError());
        return false;
    }

    bool result = text->cachedTexture.createFromSurface(renderer, *surface);
    SDL_DestroySurface(surface);
    if (!result)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text texture: %s", SDL_GetError());
        return false;
    }

    text->cachedString = text->text;
    text->isDirty = false;

    return true;
}

void RenderSystem::render(Renderer& renderer, AssetManager& assets, Scene& scene, float alpha)
{
    static std::vector<RenderNode> nodes;
    nodes.clear();

    // Retrieve all sprites
    scene.forEachEntityWith<TransformComponent, SpriteComponent>(
        [&](auto* e, auto& transform, auto& sprite)
    {
        nodes.push_back({
            &transform,
            &sprite,
            nullptr
        });
    });

    // Retrieve all texts
    scene.forEachEntityWith<TransformComponent, TextComponent>(
        [&](auto* e, auto& transform, auto& text)
    {
        nodes.push_back({
            &transform,
            nullptr,
            &text
        });
    });

    // Sort nodes by zIndex
    std::sort(nodes.begin(), nodes.end(), [](const RenderNode& a, const RenderNode& b) {
        return a.getZIndex() < b.getZIndex();
    });

    // Find active camera
    float camX = 0.0f;
    float camY = 0.0f;
    auto* camera = scene.getFirstEntityWith<TransformComponent, CameraComponent>();
    if (camera)
    {
        auto* camTransform = camera->getComponent<TransformComponent>();
        camX = camTransform->prevX + (camTransform->x - camTransform->prevX) * alpha;
        camY = camTransform->prevY + (camTransform->y - camTransform->prevY) * alpha;
    }

    // Render all nodes
    for (auto& node : nodes)
    {
        SpriteComponent* sprite = node.sprite;
        TextComponent* text = node.text;
        TransformComponent* transform = node.transform;

        float renderX = transform->prevX + (transform->x - transform->prevX) * alpha;
        float renderY = transform->prevY + (transform->y - transform->prevY) * alpha;

        if (sprite) 
        {
            float screenX = renderX - camX;
            float screenY = renderY - camY;

            Texture* texture = assets.resolve<Texture, TextureHandle>(sprite->texture);
            if (texture)
            {
                Rect dstRect = { screenX, screenY, sprite->width, sprite->height };

                if (sprite->hasSrcRect)
                {
                    Rect srcRect = { sprite->srcX, sprite->srcY, sprite->srcWidth, sprite->srcHeight };
                    renderer.drawTexture(*texture, dstRect, &srcRect, sprite->flipX);
                }
                else
                {
                    renderer.drawTexture(*texture, dstRect, nullptr, sprite->flipX);
                }
            }
        }
        else if (text)
        {
            if (text->isDirty && !rebuildTexture(text, renderer, assets)) 
            {
                continue;
            }

            if (text->cachedTexture.isValid())
            {
                Texture* texture = &text->cachedTexture;
                renderer.drawTexture(*texture, { renderX, renderY, texture->getWidth(), texture->getHeight() });
            }
        }
    }
}

} // namespace dagger
