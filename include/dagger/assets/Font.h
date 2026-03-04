#ifndef DAGGER_ASSETS_FONT_H
#define DAGGER_ASSETS_FONT_H

#include <memory>
#include <string>

struct TTF_Font;

namespace dagger
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

} // namespace dagger

#endif // DAGGER_ASSETS_FONT_H
