#ifndef DAGGER_CORE_COMPONENT_H
#define DAGGER_CORE_COMPONENT_H

#include <dagger/core/Entity.h>

namespace dagger
{

class Entity;

struct Component
{
    ~Component() = default;
    dagger::Entity* owner;
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENT_H
