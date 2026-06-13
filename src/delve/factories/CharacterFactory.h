#ifndef DELVE_FACTORIES_CHARACTER_FACTORY_H
#define DELVE_FACTORIES_CHARACTER_FACTORY_H

#include <dagger/assets/ResourceHandle.h>

namespace dagger
{
    class Entity;
    class Scene;
}

namespace delve
{

class CharacterFactory
{
public:
    static dagger::Entity* createPlayer(dagger::Scene& scene, dagger::TextureHandle texture, float x, float y);
    static dagger::Entity* createEnemy(dagger::Scene& scene, dagger::TextureHandle texture, float x, float y);
};

} // namespace delve

#endif // DELVE_FACTORIES_CHARACTER_FACTORY_H
