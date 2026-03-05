#ifndef DAGGER_CORE_GAME_OBJECT_H
#define DAGGER_CORE_GAME_OBJECT_H

namespace dagger
{

class Renderer;
class AssetManager;
class Input;

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void update(float dt, Input& input) { }
    virtual void render(Renderer& renderer, AssetManager& assets) = 0;

    void setPosition(float x, float y) { x_ = x; y_ = y; }
    float getX() const { return x_; }
    float getY() const { return y_; }

    void setActive(bool active) { isActive_ = active; }
    bool isActive() const { return isActive_; }

protected:
    float x_ = 0.0f;
    float y_ = 0.0f;
    bool isActive_ = true;
};

} // namespace dagger

#endif // DAGGER_CORE_GAME_OBJECT_H
