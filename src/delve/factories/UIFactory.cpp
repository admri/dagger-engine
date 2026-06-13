#include <factories/UIFactory.h>

#include <components/Tags.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/graphics/Color.h>

namespace delve
{

dagger::Entity* UIFactory::createFPSDisplay(dagger::Scene& scene, dagger::FontHandle font, float x, float y)
{
    auto fpsDisplay = scene.createEntity();

    auto& fpsTransform = fpsDisplay->addComponent<dagger::TransformComponent>();
    fpsTransform.setPosition(x, y);
    
    auto& fpsText = fpsDisplay->addComponent<dagger::TextComponent>();
    fpsText.setFont(font);
    fpsText.setColor(dagger::Colors::White);
    fpsText.zIndex = 100;
    
    fpsDisplay->addComponent<FPSCounterTag>();

    return fpsDisplay;
}

dagger::Entity* UIFactory::createHealthDisplay(dagger::Scene& scene, dagger::FontHandle font, float x, float y)
{
    auto hpDisplay = scene.createEntity();
    
    auto& hpTransform = hpDisplay->addComponent<dagger::TransformComponent>();
    hpTransform.setPosition(x, y);
    
    auto& hpText = hpDisplay->addComponent<dagger::TextComponent>();
    hpText.setFont(font);
    hpText.setColor(dagger::Colors::Red);
    hpText.zIndex = 100;
    
    hpDisplay->addComponent<HealthDisplayTag>();
    
    return hpDisplay;
}

} // namespace delve
