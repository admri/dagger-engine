#include <entities/Player.h>

#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

namespace delve
{

using namespace dagger;

Player::Player(dagger::TextureHandle texture) 
    : dagger::SpriteObject(texture)
    , moveSpeed_(200.0f)
{
}

void Player::update(float dt, Input& input)
{
    float moveX = 0.0f;
    float moveY = 0.0f;

    // Gather input
    if (input.isKeyDown(dagger::Key::W)) moveY -= 1.0f;
    if (input.isKeyDown(dagger::Key::S)) moveY += 1.0f;
    if (input.isKeyDown(dagger::Key::A)) moveX -= 1.0f;
    if (input.isKeyDown(dagger::Key::D)) moveX += 1.0f;

    // Normalize diagonal movement
    if (moveX != 0.0f && moveY != 0.0f)
    {
        moveX *= 0.707f;
        moveY *= 0.707f;
    }

    // Apply Movement 
    float currentX = getX();
    float currentY = getY();
    
    setPosition(currentX + (moveX * moveSpeed_ * dt), 
                currentY + (moveY * moveSpeed_ * dt));
}

} // namespace delve
