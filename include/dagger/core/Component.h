#ifndef DAGGER_CORE_COMPONENT_H
#define DAGGER_CORE_COMPONENT_H

namespace dagger
{

class Entity;

struct Component
{
    ~Component() = default;
    Entity* owner;
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENT_H
