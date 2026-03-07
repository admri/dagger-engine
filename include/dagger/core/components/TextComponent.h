#ifndef DAGGER_CORE_COMPONENTS_TEXT_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_TEXT_COMPONENT_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Component.h>
#include <dagger/graphics/Color.h>

#include <string>

namespace dagger
{

struct TextComponent : Component
{
    std::string text;
    FontHandle font;
    Color color;
    int zIndex = 100;

    std::string cachedString = "";
    TextureHandle cachedTexture;
    bool isDirty = true;

    void setText(const std::string& newText)
    {
        if (text != newText)
        {
            text = newText;
            isDirty = true;
        }
    }
};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_TEXT_COMPONENT_H
