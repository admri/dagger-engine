#include <assets/AssetManager.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

bool AssetManager::initAudio()
{
    return sounds_.init();
}

TextureHandle AssetManager::loadTexture(const std::string& id, const std::string& path)
{
    return textures_.loadTexture(id, path);
}

TextureHandle AssetManager::getTextureHandle(const std::string& id) const
{
    return textures_.getHandle(id);
}

void AssetManager::unloadTexture(const std::string& id)
{
    textures_.unloadTexture(id);
}

Texture* AssetManager::resolveTexture(TextureHandle handle)
{
    return textures_.resolve(handle);
}

FontHandle AssetManager::loadFont(const std::string& id, const std::string& path, int fontSize)
{
    return fonts_.loadFont(id, path, fontSize);
}

FontHandle AssetManager::getFontHandle(const std::string& id) const
{
    return fonts_.getHandle(id);
}

void AssetManager::unloadFont(const std::string& id)
{
    fonts_.unloadFont(id);
}

Font* AssetManager::resolveFont(FontHandle handle)
{
    return fonts_.resolve(handle);
}

SoundHandle AssetManager::loadSound(const std::string& id, const std::string& path)
{
    return sounds_.loadSound(id, path);
}

SoundHandle AssetManager::getSoundHandle(const std::string& id) const
{
    return sounds_.getHandle(id);
}

void AssetManager::unloadSound(const std::string& id)
{
    sounds_.unloadSound(id);
}

Sound* AssetManager::resolveSound(FontHandle handle)
{
    return sounds_.resolve(handle);
}

void AssetManager::clear()
{
    textures_.clear();
    fonts_.clear();
    sounds_.clear();
}
