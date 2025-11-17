#include <graphics/TextureManager.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

Texture *TextureManager::loadTexture(const std::string &id, const std::string &path, bool predecode)
{
    // Check if texture already exists
    auto it = textures_.find(id);
    if (it != textures_.end())
    {
        return it->second.get();
    }

    // Load new texture
    auto texture = std::make_unique<Texture>();
    if (!texture->loadFromFile(renderer_, path))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture %s from %s\n", id.c_str(), path.c_str());
        return nullptr;
    }

    Texture* texturePtr = texture.get();
    textures_[id] = std::move(texture);

    return texturePtr;
}

Texture* TextureManager::getTexture(const std::string& id) const
{
    auto it = textures_.find(id);
    if (it != textures_.end())
    {
        return it->second.get();
    }
    return nullptr;
}

void TextureManager::unloadTexture(const std::string& id)
{
    textures_.erase(id);
}

void TextureManager::clear()
{
    textures_.clear();
}
