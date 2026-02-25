#ifndef TEXT_H
#define TEXT_H

#include <assets/Font.h>
#include <graphics/Color.h>
#include <graphics/Renderer.h>
#include <graphics/Texture.h>

class Text
{
public:
    Text();
    Text(Font& font, const std::string& text, Color color);

    void setFont(Font& font);
    void setContent(const std::string& text);
    void setColor(Color color);

    bool rebuild(Renderer& renderer);

    void render(Renderer& renderer, int x, int y);

    float getWidth() const;
    float getHeight() const;

private:
    Font* font_;
    std::string text_;
    Color color_;

    Texture texture_;
    bool dirty_;
};

#endif // TEXT_H
