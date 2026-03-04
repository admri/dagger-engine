#include <assets/Font.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace dagger
{

void TTFFontDeleter::operator()(TTF_Font* font) const
{
    if (font) TTF_CloseFont(font);
}

bool Font::load(const std::string &path, int fontSize)
{
    ttfFont_.reset(TTF_OpenFont(path.c_str(), fontSize));

    if (!ttfFont_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to load font %s: %s\n",
                     path.c_str(),
                     SDL_GetError());
        return false;
    }

    return true;
}

void Font::destroy()
{
    if (!ttfFont_) return;

    ttfFont_.reset();
}

} // namespace dagger
