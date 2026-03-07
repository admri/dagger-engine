#include <core/systems/RenderSystem.h>

#include <assets/AssetManager.h>
#include <core/Entity.h>
#include <core/components/SpriteComponent.h>
#include <dagger/core/components/TextComponent.h>
#include <core/components/TransformComponent.h>
#include <graphics/Renderer.h>

#include <algorithm>

namespace dagger
{

void RenderSystem::render(Renderer& renderer, AssetManager& assets)
{
    std::sort(nodes_.begin(), nodes_.end(), [](const RenderNode& a, const RenderNode& b) {
        return a.getZIndex() < b.getZIndex();
    });

    for (auto& node : nodes_)
    {
        if ((node.sprite && (!node.sprite->owner || !node.sprite->owner->isActive())) ||
            (node.text && (!node.text->owner || !node.text->owner->isActive()))) {
            continue; 
        }

        if (node.sprite) 
        {
            Texture* texture = assets.resolve<Texture, TextureHandle>(node.sprite->texture);
            if (texture) renderer.drawTexture(*texture, node.transform->x, node.transform->y, nullptr);
        }
        else if (node.text)
        {
            if (node.text->isDirty) 
            {
                // Destroy the old cached texture if it exists
                if (node.text->cachedTexture.isValid())
                {
                    // Free old texture memory here
                }
                
                // Ask SDL_ttf / AssetManager to generate a brand new texture
                node.text->cachedTexture = assets.generateTextTexture(node.text->font, node.text->text);
                
                // It is now clean!
                node.text->cachedString = node.text->text;
                node.text->isDirty = false;
            }

            if (node.text->cachedTexture.isValid())
            {
                Texture* texture = assets.resolve<Texture, TextureHandle>(node.text->cachedTexture);
                renderer.drawTexture(*texture, node.transform->x, node.transform->y, nullptr);
            }
        }
    }
}

void RenderSystem::registerEntity(Entity* entity)
{
    auto* transform = entity->getComponent<TransformComponent>();

    if (!transform) return;

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
