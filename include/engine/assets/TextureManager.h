#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <assets/ResourceHandle.h>
#include <graphics/Texture.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Renderer;

class TextureManager
{
public:
    TextureManager(Renderer& renderer) : renderer_(renderer) {};

    TextureHandle loadTexture(const std::string& id, const std::string& path, bool predecode = true);
    TextureHandle getHandle(const std::string& id) const;
    void unloadTexture(const std::string& id);
    void clear();

    Texture* resolve(TextureHandle handle);

private:
    struct Slot
    {
        Texture texture;
        uint32_t generation = 0;
        bool active = false;
    };

    Renderer& renderer_;
    std::vector<Slot> slots_;
    std::unordered_map<std::string, TextureHandle> nameToHandle_;
};

#endif // TEXTURE_MANAGER_H
