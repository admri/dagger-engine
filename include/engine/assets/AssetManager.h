#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <assets/FontManager.h>
#include <assets/ResourceHandle.h>
#include <assets/SoundManager.h>
#include <assets/TextureManager.h>
#include <graphics/Renderer.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace engine
{

class Texture;
class Font;
class Sound;

class AssetManager
{
public:
    AssetManager(Renderer& renderer) : textures_(renderer), fonts_(), sounds_() {}
    AssetManager(const AssetManager&) = delete;

    bool initAudio();

    TextureHandle loadTexture(const std::string& id, const std::string& path);
    TextureHandle getTextureHandle(const std::string& id) const;
    void unloadTexture(const std::string& id);
    Texture* resolveTexture(TextureHandle handle);

    FontHandle loadFont(const std::string& id, const std::string& path, int fontSize);
    FontHandle getFontHandle(const std::string& id) const;
    void unloadFont(const std::string& id);
    Font* resolveFont(FontHandle handle);

    SoundHandle loadSound(const std::string& id, const std::string& path);
    SoundHandle getSoundHandle(const std::string& id) const;
    void unloadSound(const std::string& id);
    Sound* resolveSound(SoundHandle handle);

    void clear();

private:
    TextureManager textures_;
    FontManager fonts_;
    SoundManager sounds_;
};

} // namespace engine

#endif // ASSET_MANAGER_H
