#ifndef DAGGER_CORE_COMPONENT_H
#define DAGGER_CORE_COMPONENT_H

namespace dagger
{

class Entity;

/// @brief Base structure for all ECS components. 
/// Components should be Plain Old Data (POD) and contain no logic.
struct Component
{
    ~Component() = default;
    
    /// @brief Pointer to the Entity that currently holds this component.
    Entity* owner;
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENT_H
