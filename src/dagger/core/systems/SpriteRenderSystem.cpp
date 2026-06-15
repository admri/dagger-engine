#include <core/systems/SpriteRenderSystem.h>

#include <assets/AssetManager.h>
#include <assets/ResourceHandle.h>
#include <core/Entity.h>
#include <core/Scene.h>
#include <core/components/CameraComponent.h>
#include <core/components/SpriteComponent.h>
#include <core/components/Tags.h>
#include <core/components/TransformComponent.h>
#include <graphics/Renderer.h>
#include <graphics/Texture.h>
#include <graphics/Viewport.h>

#include <algorithm>

namespace dagger
{

void SpriteRenderSystem::render(Renderer& renderer, AssetManager& assets, Scene& scene, const Viewport& viewport, float alpha)
{
    nodes_.clear();

    scene.forEachEntityWith<TransformComponent, SpriteComponent>(
        [&](auto* e, auto& transform, auto& sprite)
    {
        if (e->hasComponent<HiddenTag>() || e->hasComponent<UIRenderTag>()) return;

        float renderX = transform.prevX + (transform.x - transform.prevX) * alpha;
        float renderY = transform.prevY + (transform.y - transform.prevY) * alpha;
        float screenX = renderX - viewport.camX;
        float screenY = renderY - viewport.camY;

        if (screenX + sprite.width  < 0 || screenX > viewport.windowWidth ||
            screenY + sprite.height < 0 || screenY > viewport.windowHeight)
        {
            return;
        }

        Color nodeTint = Colors::White;
        if (e->hasComponent<ShadowTag>())
        {
            nodeTint = Color(140, 140, 140, 255);
        }

        nodes_.push_back({ &sprite, nodeTint, screenX, screenY });
    });

    std::stable_sort(nodes_.begin(), nodes_.end(), [](const SpriteNode& a, const SpriteNode& b) {
        return a.sprite->zIndex < b.sprite->zIndex;
    });

    for (auto& node : nodes_)
    {
        Texture* texture = assets.resolve<Texture, TextureHandle>(node.sprite->texture);
        if (texture)
        {
            Rect dstRect = { node.screenX, node.screenY, node.sprite->width, node.sprite->height };
            Rect* pSrcRect = nullptr;
            Rect srcRect;

            if (node.sprite->hasSrcRect)
            {
                srcRect = { node.sprite->srcX, node.sprite->srcY, node.sprite->srcWidth, node.sprite->srcHeight };
                pSrcRect = &srcRect;
            }

            renderer.drawTexture(*texture, dstRect, pSrcRect, node.sprite->flipX, node.tint);
        }
    }
}

} // namespace dagger
