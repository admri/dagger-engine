#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <assets/Font.h>
#include <assets/ResourceHandle.h>

#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace engine
{

class FontManager
{
public:
    FontManager() = default;

    FontHandle loadFont(const std::string& id, const std::string& path, int fontSize);
    FontHandle getHandle(const std::string& id) const;
    void unloadFont(const std::string& id);
    void clear();

    Font* resolve(FontHandle handle);

private:
    struct Slot
    {
        Font font;
        uint32_t generation = 0;
        bool active = false;
    };

    std::vector<Slot> slots_;
    std::unordered_map<std::string, FontHandle> nameToHandle_;
};

} // namespace engine

#endif // FONT_MANAGER_H
