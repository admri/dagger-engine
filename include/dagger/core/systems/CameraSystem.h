#ifndef DAGGER_CAMERA_SYSTEM_H
#define DAGGER_CAMERA_SYSTEM_H

namespace dagger
{

class Application;
class Scene;

/// @brief Updates the viewport boundaries and camera position, typically following a target entity (e.g., the player).
class CameraSystem
{
public:
    void update(Application& app, Scene& scene);
};

} // namespace dagger

#endif // DAGGER_CAMERA_SYSTEM_H
