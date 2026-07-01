#ifndef DAGGER_CORE_SYSTEMS_ANIMATION_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_ANIMATION_SYSTEM_H

namespace dagger
{

class Scene;

/// @brief Advances the current frame of entities with an AnimationComponent based on delta time.
class AnimationSystem
{
public:
    /// @brief Updates animation timers and switches frames if necessary.
    void update(float dt, Scene& scene);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_ANIMATION_SYSTEM_H
