#ifndef DAGGER_CORE_SYSTEMS_ANIMATION_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_ANIMATION_SYSTEM_H

namespace dagger
{

class Scene;

class AnimationSystem
{
public:
    void update(float dt, Scene& scene);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_ANIMATION_SYSTEM_H
