#ifndef DELVE_SYSTEMS_FOV_SYSTEM_H
#define DELVE_SYSTEMS_FOV_SYSTEM_H

namespace dagger
{
    class Scene;
}

namespace delve
{

class TileMap;

class FOVSystem
{
public:
    void update(float dt, dagger::Scene& scene, TileMap& tileMap);
};

} // namespace delve

#endif // DELVE_SYSTEMS_FOV_SYSTEM_H
