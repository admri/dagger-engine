#ifndef FONT_H
#define FONT_H

#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <string>

struct TTFFontDeleter
{
    void operator()(TTF_Font* font) const
    {
        if (font) TTF_CloseFont(font);
    }
};

class Font
{
public:
    bool loadFromFile(const std::string& path, int fontSize);
    TTF_Font* get() const { return ttfFont_.get(); }

private:
    std::unique_ptr<TTF_Font, TTFFontDeleter> ttfFont_;
};

#endif // FONT_H
