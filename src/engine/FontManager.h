#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "Font.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <string>
#include <unordered_map>

class FontManager
{
public:
    FontManager() = default;
    ~FontManager() { clear(); }

    Font* loadFont(const std::string& id, const std::string& path, int fontSize);
    Font* getFont(const std::string& id) const;
    void unloadFont(const std::string& id);

    void clear();

private:
    std::unordered_map<std::string, std::unique_ptr<Font>> fonts_;
};

#endif // FONT_MANAGER_H
