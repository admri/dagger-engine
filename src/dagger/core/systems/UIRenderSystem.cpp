#include <core/systems/UIRenderSystem.h>

#include <assets/AssetManager.h>
#include <core/Scene.h>
#include <core/components/SpriteComponent.h>
#include <core/components/Tags.h>
#include <core/components/TextComponent.h>
#include <core/components/TransformComponent.h>
#include <graphics/Color.h>
#include <graphics/Rect.h>
#include <graphics/Renderer.h>
#include <graphics/Texture.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace dagger
{

namespace
{
    SDL_Color toSDL(Color c)
    {
        return {c.r, c.g, c.b, c.a};
    }

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
}

struct UINode
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

void UIRenderSystem::render(Renderer& renderer, AssetManager& assets, Scene& scene, float alpha)
{
    static std::vector<UINode> nodes;
    nodes.clear();

    // Retrieve all UI sprites
    scene.forEachEntityWith<TransformComponent, SpriteComponent, UIRenderTag>(
        [&](auto* e, auto& transform, auto& sprite, auto& tag)
    {
        nodes.push_back({ &transform, &sprite, nullptr });
    });

    // Retrieve all UI texts
    scene.forEachEntityWith<TransformComponent, TextComponent, UIRenderTag>(
        [&](auto* e, auto& transform, auto& text, auto& tag)
    {
        nodes.push_back({ &transform, nullptr, &text });
    });

    // Sort all by zIndex
    std::sort(nodes.begin(), nodes.end(), [](const UINode& a, const UINode& b) {
        return a.getZIndex() < b.getZIndex();
    });

    // Render
    for (auto& node : nodes)
    {
        float screenX = node.transform->prevX + (node.transform->x - node.transform->prevX) * alpha;
        float screenY = node.transform->prevY + (node.transform->y - node.transform->prevY) * alpha;

        if (node.sprite)
        {
            Texture* texture = assets.resolve<Texture, TextureHandle>(node.sprite->texture);
            if (texture)
            {
                Rect dstRect = { screenX, screenY, node.sprite->width, node.sprite->height };
                if (node.sprite->hasSrcRect)
                {
                    Rect srcRect = { node.sprite->srcX, node.sprite->srcY, node.sprite->srcWidth, node.sprite->srcHeight };
                    renderer.drawTexture(*texture, dstRect, &srcRect, node.sprite->flipX);
                }
                else
                {
                    renderer.drawTexture(*texture, dstRect, nullptr, node.sprite->flipX);
                }
            }
        }
        else if (node.text)
        {
            if (node.text->isDirty && !rebuildTexture(node.text, renderer, assets)) continue;

            if (node.text->cachedTexture.isValid())
            {
                renderer.drawTexture(node.text->cachedTexture, { screenX, screenY, node.text->cachedTexture.getWidth(), node.text->cachedTexture.getHeight() });
            }
        }
    }
}

} // namespace dagger
