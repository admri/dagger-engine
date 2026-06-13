#ifndef DELVE_FACTORIES_UI_FACTORY_H
#define DELVE_FACTORIES_UI_FACTORY_H

#include <dagger/assets/ResourceHandle.h>

namespace dagger
{
    class Entity;
    class Scene;
}

namespace delve
{

class UIFactory
{
public:
    static dagger::Entity* createFPSDisplay(dagger::Scene& scene, dagger::FontHandle font, float x, float y);
    static dagger::Entity* createHealthDisplay(dagger::Scene& scene, dagger::FontHandle font, float x, float y);
};

} // namespace delve

#endif // DELVE_FACTORIES_UI_FACTORY_H
