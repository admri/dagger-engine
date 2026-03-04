#ifndef DAGGER_CORE_TEXT_OBJECT_H
#define DAGGER_CORE_TEXT_OBJECT_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/GameObject.h>
#include <dagger/graphics/Color.h>
#include <dagger/graphics/Texture.h>

#include <string>

namespace dagger
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

} // namespace dagger

#endif // DAGGER_CORE_TEXT_OBJECT_H
