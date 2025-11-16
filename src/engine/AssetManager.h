#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "FontManager.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"

#include <memory>
#include <string>
#include <unordered_map>

class AssetManager
{
public:
    AssetManager(Renderer& renderer) : textures_(renderer), fonts_(), sounds_() {}
    AssetManager(const AssetManager&) = delete;
    ~AssetManager() { clear(); }

    Texture* loadTexture(const std::string& id, const std::string& path);
    Texture* getTexture(const std::string& id) const;
    void unloadTexture(const std::string& id);

    Font* loadFont(const std::string& id, const std::string& path, int fontSize);
    Font* getFont(const std::string& id) const;
    void unloadFont(const std::string& id);

    Sound* loadSound(const std::string& id, const std::string& path);
    Sound* getSound(const std::string& id) const;
    void unloadSound(const std::string& id);

    void clear();

private:
    TextureManager textures_;
    FontManager fonts_;
    SoundManager sounds_;
};

#endif // ASSET_MANAGER_H
