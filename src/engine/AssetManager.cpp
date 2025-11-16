#include "AssetManager.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

Texture* AssetManager::loadTexture(const std::string& id, const std::string& path)
{
    textures_.loadTexture(id, path);
    return getTexture(id);
}

Texture* AssetManager::getTexture(const std::string& id) const
{
    return textures_.getTexture(id);
}

void AssetManager::unloadTexture(const std::string& id)
{
    textures_.unloadTexture(id);
}

Font* AssetManager::loadFont(const std::string& id, const std::string& path, int fontSize)
{
    fonts_.loadFont(id, path, fontSize);
    return getFont(id);
}

Font* AssetManager::getFont(const std::string& id) const
{
    return fonts_.getFont(id);
}

void AssetManager::unloadFont(const std::string& id)
{
    fonts_.unloadFont(id);
}

Sound* AssetManager::loadSound(const std::string& id, const std::string& path)
{
    sounds_.loadSound(id, path);
    return getSound(id);
}

Sound* AssetManager::getSound(const std::string& id) const
{
    return sounds_.getSound(id);
}

void AssetManager::unloadSound(const std::string& id)
{
    sounds_.unloadSound(id);
}

void AssetManager::clear()
{
    textures_.clear();
    fonts_.clear();
    sounds_.clear();
}
