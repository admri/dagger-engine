#include <core/TextObject.h>

#include <assets/Font.h>
#include <graphics/Renderer.h>

#include <SDL3/SDL_error.h>
#include <SDL3_ttf/SDL_ttf.h>

TextObject::TextObject()
    : font_(nullptr)
    , color_(Colors::Black)
{
}

TextObject::TextObject(Font& font, const std::string& text, Color color)
    : font_(&font)
    , text_(text)
    , color_(color)
{
}

void TextObject::setFont(Font& font)
{
    font_ = &font;
    dirty_ = true;
}

void TextObject::setText(const std::string& text)
{
    if (text_ == text)
    {
        return;
    }

    text_ = text;
    dirty_ = true;
}

void TextObject::setColor(Color color)
{
    if (color_ == color)
    {
        return;
    }

    color_ = color;
    dirty_ = true;
}

void TextObject::render(Renderer& renderer)
{
    if (dirty_ && !rebuildTexture(renderer))
    {
        return;
    }

    renderer.drawTexture(cachedTexture_, x_, y_);
}

bool TextObject::rebuildTexture(Renderer& renderer)
{
    if (!font_) return false;
    if (!dirty_) return true;

    SDL_Surface* surface = TTF_RenderText_Blended(
        font_->get(),
        text_.c_str(),
        text_.length(),
        color_
    );
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text surface: %s", SDL_GetError());
        return false;
    }

    bool result = cachedTexture_.createFromSurface(renderer, *surface);
    SDL_DestroySurface(surface);

    if (result)
    {
        dirty_ = false;
    }

    return result;
}
