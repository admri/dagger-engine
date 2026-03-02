#include <assets/FontManager.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

FontHandle FontManager::loadFont(const std::string& id, const std::string& path, int fontSize)
{
    auto it = nameToHandle_.find(id);
    if (it != nameToHandle_.end())
    {
        return it->second;
    }

    for (size_t i = 0; i < slots_.size(); ++i)
    {
        if (!slots_[i].active)
        {
            if (!slots_[i].font.loadFromFile(path, fontSize))
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                            "Failed to load font %s from %s\n",
                            id.c_str(),
                            path.c_str());
                return FontHandle{};
            }

            slots_[i].active = true;
            slots_[i].generation++;

            FontHandle handle = { static_cast<uint32_t>(i), slots_[i].generation };
            nameToHandle_[id] = handle;
            return handle;
        }
    }

    Slot newSlot;
    if (!newSlot.font.loadFromFile(path, fontSize))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to load font %s from %s\n",
                    id.c_str(),
                    path.c_str());
        return FontHandle{};
    }

    newSlot.active = true;
    newSlot.generation = 1;

    slots_.push_back(std::move(newSlot));

    uint32_t newIndex = static_cast<uint32_t>(slots_.size() - 1);
    FontHandle handle = { newIndex, 1 };

    nameToHandle_[id] = handle;
    return handle;
}

FontHandle FontManager::getHandle(const std::string& id) const
{
    auto it = nameToHandle_.find(id);
    if (it != nameToHandle_.end())
    {
        return it->second;
    }
    return FontHandle{};
}

void FontManager::unloadFont(const std::string& id)
{
    auto it = nameToHandle_.find(id);
    if (it == nameToHandle_.end()) return;

    FontHandle handle = it->second;

    if (handle.isValid() && handle.index < slots_.size())
    {
        Slot& slot = slots_[handle.index];

        if (slot.active && slot.generation == handle.generation)
        {
            slot.active = false;
        }
    }

    nameToHandle_.erase(id);
}

void FontManager::clear()
{
    slots_.clear();
}

Font* FontManager::resolve(FontHandle handle)
{
    if (!handle.isValid() || handle.index >= slots_.size())
    {
        return nullptr;
    }

    Slot& slot = slots_[handle.index];

    if (slot.active && slot.generation == handle.generation)
    {
        return &slot.font;
    }

    return nullptr;
}
