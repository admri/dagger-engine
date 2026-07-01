#ifndef DAGGER_CORE_SYSTEMS_DEBUG_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_DEBUG_RENDER_SYSTEM_H

#include <dagger/core/Scene.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/graphics/Viewport.h>

namespace dagger
{

/// @brief Utility system that renders visual outlines of colliders and invisible boundaries for debugging purposes.
class DebugRenderSystem
{
public:
    /// @brief Draws green wireframe rectangles over all entities possessing a BoxColliderComponent.
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
