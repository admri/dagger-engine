#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <graphics/Renderer.h>
#include <graphics/Texture.h>

#include <memory>
#include <string>
#include <unordered_map>

class Renderer;

class TextureManager
{
public:
    TextureManager(Renderer& renderer) : renderer_(renderer) {};

    Texture* loadTexture(const std::string& id, const std::string& path, bool predecode = true);
    Texture* getTexture(const std::string& id) const;
    void unloadTexture(const std::string& id);

    void clear();

private:
    Renderer& renderer_;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures_;
};

#endif // TEXTURE_MANAGER_H
