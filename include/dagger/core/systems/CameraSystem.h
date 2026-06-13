#ifndef DAGGER_CAMERA_SYSTEM_H
#define DAGGER_CAMERA_SYSTEM_H

namespace dagger
{

class Application;
class Entity;

class CameraSystem
{
public:
    void update(Application& app, Entity* camera, Entity* target);
};

} // namespace dagger

#endif // DAGGER_CAMERA_SYSTEM_H
