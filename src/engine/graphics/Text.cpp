#include <graphics/Text.h>

Text::Text()
    : font_(nullptr)
    , text_()
    , color_(Colors::Black)
    , dirty_(true)
{
}

Text::Text(Font& font,
           const std::string& text,
           Color color)
    : font_(&font)
    , text_(text)
    , color_(color)
    , dirty_(true)
{
}

void Text::setFont(Font &font)
{
    font_ = &font;
    dirty_ = true;
}

void Text::setContent(const std::string &text)
{
    if (text_ == text)
    {
        return;
    }

    text_ = text;
    dirty_ = true;
}

void Text::setColor(Color color)
{
    if (color_ == color)
    {
        return;
    }

    color_ = color;
    dirty_ = true;
}

bool Text::rebuild(Renderer &renderer)
{
    if (!font_)
    {
        return false;
    }
    if (!dirty_)
    {
        return true;
    }

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

    bool result = texture_.createFromSurface(renderer, *surface);
    SDL_DestroySurface(surface);

    if (result)
    {
        dirty_ = false;
    }

    return result;
}

void Text::render(Renderer &renderer, int x, int y)
{
    if (dirty_)
    {
        rebuild(renderer);
    }

    texture_.render(renderer, x, y);
}

float Text::getWidth() const
{
    return texture_.getWidth();
}

float Text::getHeight() const
{
    return texture_.getHeight();
}
