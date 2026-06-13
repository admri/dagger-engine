#include <core/systems/CameraSystem.h>

#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/SpriteComponent.h>

namespace dagger
{

void CameraSystem::update(Application& app, Entity* camera, Entity* target)
{
    if (!camera || !target) return;

    auto* camTransform = camera->getComponent<TransformComponent>();
    auto* targetTransform = target->getComponent<TransformComponent>();
    auto* targetSprite = target->getComponent<SpriteComponent>();

    if (camTransform && targetTransform && targetSprite)
    {
        camTransform->prevX = camTransform->x;
        camTransform->prevY = camTransform->y;

        float screenW = static_cast<float>(app.getWindowWidth());
        float screenH = static_cast<float>(app.getWindowHeight());

        // Center the camera on the target
        camTransform->x = targetTransform->x - (screenW / 2.0f) + (targetSprite->width / 2.0f);
        camTransform->y = targetTransform->y - (screenH / 2.0f) + (targetSprite->height / 2.0f);
    }
}

} // namespace dagger
