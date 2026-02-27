#ifndef FONT_H
#define FONT_H

#include <memory>
#include <string>

struct TTF_Font;

struct TTFFontDeleter
{
    void operator()(TTF_Font* font) const;
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
