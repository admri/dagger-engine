#include <core/systems/RenderSystem.h>

#include <assets/AssetManager.h>
#include <core/Entity.h>
#include <dagger/core/components/CameraComponent.h>
#include <core/components/SpriteComponent.h>
#include <core/components/TextComponent.h>
#include <core/components/TransformComponent.h>
#include <graphics/Renderer.h>
#include <graphics/Texture.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>

namespace dagger
{

namespace
{

SDL_Color toSDL(Color c)
{
    return {c.r, c.g, c.b, c.a};
}

} // namespace

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

void RenderSystem::render(Renderer& renderer, AssetManager& assets, float alpha)
{
    std::sort(nodes_.begin(), nodes_.end(), [](const RenderNode& a, const RenderNode& b) {
        return a.getZIndex() < b.getZIndex();
    });

    float camX = 0.0f;
    float camY = 0.0f;

    if (activeCamera_ && activeCamera_->isActive())
    {
        auto* camTransform = activeCamera_->getComponent<TransformComponent>();
        if (camTransform)
        {
            camX = camTransform->prevX + (camTransform->x - camTransform->prevX) * alpha;
            camY = camTransform->prevY + (camTransform->y - camTransform->prevY) * alpha;
        }
    }

    for (auto& node : nodes_)
    {
        SpriteComponent* sprite = node.sprite;
        TextComponent* text = node.text;
        TransformComponent* transform = node.transform;

        if ((node.sprite && (!node.sprite->owner || !node.sprite->owner->isActive())) ||
            (node.text && (!node.text->owner || !node.text->owner->isActive()))) {
            continue; 
        }

        float renderX = transform->prevX + (transform->x - transform->prevX) * alpha;
        float renderY = transform->prevY + (transform->y - transform->prevY) * alpha;

        if (sprite) 
        {
            float screenX = renderX - camX;
            float screenY = renderY - camY;

            Texture* texture = assets.resolve<Texture, TextureHandle>(sprite->texture);
            if (texture)
            {
                renderer.drawTexture(*texture, { screenX, screenY, sprite->width, sprite->height });
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

void RenderSystem::registerEntity(Entity* entity)
{
    auto* transform = entity->getComponent<TransformComponent>();
    if (!transform) return;

    if (entity->hasComponent<CameraComponent>())
    {
        activeCamera_ = entity;
    }

    auto* sprite = entity->getComponent<SpriteComponent>();
    auto* text = entity->getComponent<TextComponent>();

    if (sprite)
    {
        nodes_.push_back({ transform, sprite, nullptr });
    }

    if (text)
    {
        nodes_.push_back({ transform, nullptr, text });
    }
}

} // namespace dagger
