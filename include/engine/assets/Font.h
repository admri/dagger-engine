#ifndef FONT_H
#define FONT_H

#include <memory>
#include <string>

struct TTF_Font;

namespace engine
{

struct TTFFontDeleter
{
    void operator()(TTF_Font* font) const;
};

class Font
{
public:
    bool load(const std::string& path, int fontSize);
    TTF_Font* get() const { return ttfFont_.get(); }
    void destroy();

private:
    std::unique_ptr<TTF_Font, TTFFontDeleter> ttfFont_;
};

} // namespace engine

#endif // FONT_H
