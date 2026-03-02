#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class Renderer;
class AssetManager;

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void update(float dt) { }
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

#endif // GAME_OBJECT_H
