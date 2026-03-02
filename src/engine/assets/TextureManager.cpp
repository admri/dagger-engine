#include <assets/TextureManager.h>

#include <graphics/Renderer.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

TextureHandle TextureManager::loadTexture(const std::string& id, const std::string& path, bool predecode)
{
    // Check if texture already exists
    auto it = nameToHandle_.find(id);
    if (it != nameToHandle_.end())
    {
        return it->second;
    }

    // Find an empty slot
    for (size_t i = 0; i < slots_.size(); ++i)
    {
        if (!slots_[i].active)
        {
            if (!slots_[i].texture.loadFromFile(renderer_, path))
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                            "Failed to load texture %s from %s\n",
                            id.c_str(),
                            path.c_str());
                return TextureHandle{};
            }

            slots_[i].active = true;
            slots_[i].generation++;

            TextureHandle handle = { static_cast<uint32_t>(i), slots_[i].generation };
            nameToHandle_[id] = handle;
            return handle;
        }
    }

    // Else, create a new one
    Slot newSlot;
    if (!newSlot.texture.loadFromFile(renderer_, path))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to load texture %s from %s\n",
                    id.c_str(),
                    path.c_str());
        return TextureHandle{};
    }

    newSlot.active = true;
    newSlot.generation = 1;

    slots_.push_back(std::move(newSlot));

    uint32_t newIndex = static_cast<uint32_t>(slots_.size() - 1);
    TextureHandle handle = { newIndex, 1 };

    nameToHandle_[id] = handle;
    return handle;
}

TextureHandle TextureManager::getHandle(const std::string& id) const
{
    auto it = nameToHandle_.find(id);
    if (it != nameToHandle_.end())
    {
        return it->second;
    }

    return TextureHandle{};
}

void TextureManager::unloadTexture(const std::string& id)
{
    auto it = nameToHandle_.find(id);
    if (it == nameToHandle_.end()) return;

    TextureHandle handle = it->second;

    if (handle.isValid() && handle.index < slots_.size())
    {
        Slot& slot = slots_[handle.index];

        if (slot.active && slot.generation == handle.generation)
        {
            slot.texture.destroy();
            slot.active = false;
        }
    }

    nameToHandle_.erase(id);
}

void TextureManager::clear()
{
    slots_.clear();
}

Texture* TextureManager::resolve(TextureHandle handle)
{
    if (!handle.isValid() || handle.index >= slots_.size())
    {
        return nullptr;
    }

    Slot& slot = slots_[handle.index];

    if (slot.active && slot.generation == handle.generation)
    {
        return &slot.texture;
    }

    return nullptr;
}
