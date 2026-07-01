#include <systems/LightingRenderSystem.h>

#include <components/FOVComponent.h>
#include <components/PlayerInputComponent.h>
#include <world/TileMap.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/graphics/Viewport.h>

#include <cmath>

namespace delve
{

void LightingRenderSystem::init(dagger::Application& app)
{
    dagger::Renderer& renderer = app.getRenderer();

    // Create the occlusion target
    occlusionTarget_.createRenderTarget(
        renderer, 
        renderer.getWidth(), 
        renderer.getHeight()
    );

    // Load the fragment shader
    std::string shaderPath = app.getAssets().getAbsolutePath("shaders/lighting");
    lightingShader_ = renderer.loadFragmentShader(shaderPath, sizeof(LightUniforms));
}

void LightingRenderSystem::render(dagger::Renderer& renderer, dagger::Scene& scene, const TileMap& tileMap, const dagger::Viewport& viewport)
{
    if (!lightingShader_.shader) return;

    // Update the occlusion target on window resize
    if (occlusionTarget_.getWidth() != viewport.windowWidth || 
        occlusionTarget_.getHeight() != viewport.windowHeight)
    {
        occlusionTarget_.createRenderTarget(renderer, viewport.windowWidth, viewport.windowHeight);
    }

    auto* player = scene.getFirstEntityWith<dagger::TransformComponent, PlayerInputComponent, FOVComponent>();
    FOVComponent* playerFov = player ? player->getComponent<FOVComponent>() : nullptr;

    //
    //  Drawing walls onto the occlusion texture
    //
    renderer.setRenderTarget(&occlusionTarget_);
    renderer.clear(dagger::Colors::Black);

    float tileSize = tileMap.getTileSize();
    
    // Only iterate through the visible part of the map
    int startX = static_cast<int>(std::floor(viewport.camX / tileSize));
    int startY = static_cast<int>(std::floor(viewport.camY / tileSize));
    int endX = static_cast<int>(std::floor((viewport.camX + viewport.windowWidth) / tileSize)) + 1;
    int endY = static_cast<int>(std::floor((viewport.camY + viewport.windowHeight) / tileSize)) + 1;

    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            bool isWall = (tileMap.getTile(x, y) == TileType::Wall);

            if (isWall)
            {
                // Draw the wall
                dagger::Rect rect{
                    (x * tileSize) - viewport.camX,
                    (y * tileSize) - viewport.camY,
                    tileSize,
                    tileSize
                };
                
                renderer.fillRect(rect, dagger::Colors::White);
            }
        }
    }

    // 
    // Applying the shader onto the screen
    //
    renderer.setRenderTarget(nullptr);

    // Create the uniform data
    LightUniforms lightData{};
    lightData.aspectRatio = static_cast<float>(viewport.windowWidth) / static_cast<float>(viewport.windowHeight);
    lightData.lightRadius = 0.7f; // Default fallback radius
    lightData.lightX = 0.5f;      // Default to center of screen
    lightData.lightY = 0.5f;      // Default to center of screen

    // Find the player and calculate their relative position on screen (0.0 to 1.0)
    if (player)
    {
        auto* transform = player->getComponent<dagger::TransformComponent>();
        auto* collider = player->getComponent<dagger::BoxColliderComponent>();
        
        float offsetX = collider ? collider->offsetX + (collider->width / 2.0f) : 0.0f;
        float offsetY = collider ? collider->offsetY + (collider->height / 2.0f) : 0.0f;

        float screenX = (transform->x + offsetX) - viewport.camX;
        float screenY = (transform->y + offsetY) - viewport.camY;
        
        lightData.lightX = screenX / static_cast<float>(viewport.windowWidth);
        lightData.lightY = screenY / static_cast<float>(viewport.windowHeight);
        if (playerFov) 
        {
            // Make the light radius a bit smaller to give CPU time to render in enemies
            // and such before the light shader gets to them
            float safeRadius = playerFov->radius - tileMap.getTileSize();
            lightData.lightRadius = safeRadius / static_cast<float>(viewport.windowHeight);
        }
    }

    // Turn on the shader with the light data
    renderer.setShader(&lightingShader_);
    renderer.setShaderUniforms(&lightingShader_, &lightData, sizeof(lightData));

    // Draw the filter with the shader on
    dagger::Rect screenRect{ 0.0f, 0.0f, static_cast<float>(viewport.windowWidth), static_cast<float>(viewport.windowHeight) };
    renderer.drawTexture(occlusionTarget_, screenRect);

    // Turn off the shader
    renderer.setShader(nullptr);
}

} // namespace delve
