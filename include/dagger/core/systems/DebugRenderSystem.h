#ifndef DAGGER_CORE_SYSTEMS_DEBUG_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_DEBUG_RENDER_SYSTEM_H

#include <dagger/core/Scene.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/graphics/Viewport.h>

namespace dagger
{

class DebugRenderSystem
{
public:
    void render(Renderer& renderer, Scene& scene, const Viewport& viewport)
    {
        scene.forEachEntityWith<TransformComponent, BoxColliderComponent>(
            [&](auto* entity, auto& transform, auto& collider)
        {
            float screenX = transform.x - viewport.camX + collider.offsetX;
            float screenY = transform.y - viewport.camY + collider.offsetY;

            Color debugColor = Colors::Green; 
            
            Rect debugRect = { 
                screenX,
                screenY,
                collider.width, 
                collider.height 
            };

            renderer.drawRect(debugRect, debugColor);
        });
    }
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_DEBUG_RENDER_SYSTEM_H
