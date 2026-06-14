#ifndef DAGGER_CORE_COMPONENTS_TEXT_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_TEXT_COMPONENT_H

#include <dagger/assets/PrefabFactory.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>
#include <dagger/graphics/Color.h>
#include <dagger/graphics/Texture.h>

#include <string>

namespace dagger
{

struct TextComponent : public Component
{
    std::string text = "";
    FontHandle font = {};
    Color color = Colors::Black;
    int zIndex = 100;

    std::string cachedString = "";
    Texture cachedTexture;
    bool isDirty = true;

    void setText(const std::string& newText)
    {
        if (text != newText)
        {
            text = newText;
            isDirty = true;
        }
    }

    void setFont(FontHandle newFont)
    {
        if (font.index != newFont.index || font.generation != newFont.generation)
        {
            font = newFont;
            isDirty = true;
        }
    }

    void setColor(Color newColor)
    {
        if (color != newColor)
        {
            color = newColor;
            isDirty = true;
        }
    }
};

DAGGER_SERIALIZE_COMPONENT(TextComponent, text, color, zIndex)

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_TEXT_COMPONENT_H
