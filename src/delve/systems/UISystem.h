#ifndef DELVE_SYSTEMS_UI_SYSTEM_H
#define DELVE_SYSTEMS_UI_SYSTEM_H

#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TextComponent.h>

namespace delve
{

class UISystem
{
public:
    template <typename Tag, typename TextGeneratorFunc>
    void updateElement(dagger::Scene& scene, TextGeneratorFunc generator)
    {
        auto elements = scene.getEntitiesWith<dagger::TextComponent, Tag>();

        scene.forEachEntityWith<dagger::TextComponent, Tag>(
            [&](auto* entity)
        {
            auto* textComp = entity->getComponent<dagger::TextComponent>();
            textComp->setText(generator());
        });
    }
};

} // namespace delve

#endif // DELVE_SYSTEMS_UI_SYSTEM_H
