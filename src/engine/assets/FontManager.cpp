#include <assets/FontManager.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

Font* FontManager::loadFont(const std::string& id, const std::string& path, int fontSize)
{
    auto it = fonts_.find(id);
    if (it != fonts_.end())
    {
        return it->second.get();
    }

    auto font = std::make_unique<Font>();
    if (!font->loadFromFile(path, fontSize))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font %s from %s\n", id.c_str(), path.c_str());
        return nullptr;
    }

    Font* fontPtr = font.get();
    fonts_[id] = std::move(font);

    return fontPtr;
}

Font* FontManager::getFont(const std::string& id) const
{
    auto it = fonts_.find(id);
    if (it != fonts_.end())
    {
        return it->second.get();
    }
    return nullptr;
}

void FontManager::unloadFont(const std::string& id)
{
    fonts_.erase(id);
}

void FontManager::clear()
{
    fonts_.clear();
}
