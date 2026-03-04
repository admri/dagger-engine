#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <engine/assets/ResourceHandle.h>
#include <engine/core/GameObject.h>
#include <engine/graphics/Color.h>
#include <engine/graphics/Texture.h>

#include <string>

namespace engine
{

class Font;
class Renderer;

class TextObject : public GameObject
{
public:
    TextObject();
    explicit TextObject(FontHandle handle, const std::string& text, Color color);

    void setFont(FontHandle handle);
    void setText(const std::string& text);
    void setColor(Color color);

    void render(Renderer& renderer, AssetManager& assets) override;

private:
    FontHandle fontHandle_;
    std::string text_;
    Color color_;
    Texture cachedTexture_;
    bool dirty_ = true;

    bool rebuildTexture(Renderer& renderer, AssetManager& assets);
};

} // namespace engine

#endif // TEXT_OBJECT_H
