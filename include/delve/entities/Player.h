#ifndef DELVE_ENTITIES_PLAYER_H
#define DELVE_ENTITIES_PLAYER_H

#include <dagger/core/SpriteObject.h>

namespace dagger { class Input; }

namespace delve
{

class Player : public dagger::SpriteObject
{
public:
    Player(dagger::TextureHandle handle);

    void update(float dt, dagger::Input& input) override;

private:
    float moveSpeed_;
};

} // namespace delve

#endif // DELVE_ENTITIES_PLAYER_H
