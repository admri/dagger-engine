#include <core/systems/SpriteRenderSystem.h>

#include <assets/AssetManager.h>
#include <assets/ResourceHandle.h>
#include <core/Entity.h>
#include <core/Scene.h>
#include <core/components/CameraComponent.h>
#include <core/components/SpriteComponent.h>
#include <core/components/Tags.h>
#include <core/components/TransformComponent.h>
#include <graphics/Color.h>
#include <graphics/Renderer.h>
#include <graphics/Texture.h>

#include <algorithm>
#include <vector>

namespace dagger
{

struct SpriteNode
{
    TransformComponent* transform;
    SpriteComponent* sprite;
    Color tint; 
};

void SpriteRenderSystem::render(Renderer& renderer, AssetManager& assets, Scene& scene, float alpha)
{
    static std::vector<SpriteNode> nodes;
    nodes.clear();

    scene.forEachEntityWith<TransformComponent, SpriteComponent>(
        [&](auto* e, auto& transform, auto& sprite)
    {
        if (e->hasComponent<HiddenTag>() || e->hasComponent<UIRenderTag>()) return;

        Color nodeTint = Colors::White;
        if (e->hasComponent<ShadowTag>())
        {
            nodeTint = Color(140, 140, 140, 255);
        }

        nodes.push_back({ &transform, &sprite, nodeTint });
    });

    std::sort(nodes.begin(), nodes.end(), [](const SpriteNode& a, const SpriteNode& b) {
        return a.sprite->zIndex < b.sprite->zIndex;
    });

    float camX = 0.0f;
    float camY = 0.0f;
    auto* camera = scene.getFirstEntityWith<TransformComponent, CameraComponent>();
    if (camera)
    {
        auto* camTransform = camera->getComponent<TransformComponent>();
        camX = camTransform->prevX + (camTransform->x - camTransform->prevX) * alpha;
        camY = camTransform->prevY + (camTransform->y - camTransform->prevY) * alpha;
    }

    for (auto& node : nodes)
    {
        float renderX = node.transform->prevX + (node.transform->x - node.transform->prevX) * alpha;
        float renderY = node.transform->prevY + (node.transform->y - node.transform->prevY) * alpha;

        float screenX = renderX - camX;
        float screenY = renderY - camY;

        Texture* texture = assets.resolve<Texture, TextureHandle>(node.sprite->texture);
        if (texture)
        {
            Rect dstRect = { screenX, screenY, node.sprite->width, node.sprite->height };
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
