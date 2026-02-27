#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <core/GameObject.h>
#include <graphics/Color.h>
#include <graphics/Texture.h>

#include <string>

class Font;
class Renderer;

class TextObject : public GameObject
{
public:
    TextObject();
    explicit TextObject(Font& font, const std::string& text, Color color);

    void setFont(Font& font);
    void setText(const std::string& text);
    void setColor(Color color);

    void render(Renderer& renderer) override;

private:
    Font* font_;
    std::string text_;
    Color color_;
    Texture cachedTexture_;
    bool dirty_ = true;

    bool rebuildTexture(Renderer& renderer);
};

#endif // TEXT_OBJECT_H
