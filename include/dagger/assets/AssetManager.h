#ifndef DAGGER_ASSETS_ASSET_MANAGER_H
#define DAGGER_ASSETS_ASSET_MANAGER_H

#include <dagger/assets/Font.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/assets/ResourceManager.h>
#include <dagger/assets/ResourceTraits.h>
#include <dagger/audio/AudioSystem.h>
#include <dagger/audio/Sound.h>
#include <dagger/graphics/Texture.h>

#include <memory>
#include <string>
#include <utility>

namespace dagger
{

class Renderer;

class AssetManager
{
public:
    AssetManager(Renderer& renderer, AudioSystem& audio);
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::string getAbsolutePath(const std::string& relativePath) const
    {
        return basePath_ + relativePath;
    }

    template <typename T, typename... Args>
    auto load(const std::string& id, const std::string& filePath = "", Args&&... args)
    {
        auto& manager = getManager<T>();
        if (manager.has(id)) return manager.getHandle(id);

        std::string absolutePath = getAbsolutePath(filePath);

        T newRes;
        bool success = false;

        if constexpr (ResourceTraits<T>::needsRenderer && ResourceTraits<T>::needsMixer)
        {
            success = newRes.load(renderer_, audio_.getMixer(), absolutePath, std::forward<Args>(args)...);
        }
        else if constexpr (ResourceTraits<T>::needsRenderer)
        {
            success = newRes.load(renderer_, absolutePath, std::forward<Args>(args)...);
        }
        else if constexpr (ResourceTraits<T>::needsMixer)
        {
            success = newRes.load(audio_.getMixer(), absolutePath, std::forward<Args>(args)...);
        }
        else
        {
            success = newRes.load(absolutePath, std::forward<Args>(args)...);
        }

        if (!success)
        {
            return decltype(manager.getHandle(id)){};
        }

        return manager.add(id, std::move(newRes));
    }

    template <typename T, typename... Args>
    void replace(const std::string& id, Args&&... args)
    {
        auto& manager = getManager<T>();
        if (!manager.has(id)) return;

        T newRes;
        if constexpr (ResourceTraits<T>::needsRenderer &&
                      ResourceTraits<T>::needsMixer)
        {
            if (!newRes.load(renderer_, audio_.getMixer(), std::forward<Args>(args)...)) return;
        }
        else if constexpr (ResourceTraits<T>::needsRenderer)
        {
            if (!newRes.load(renderer_, std::forward<Args>(args)...)) return;
        }
        else if constexpr (ResourceTraits<T>::needsMixer)
        {
            if (!newRes.load(audio_.getMixer(), std::forward<Args>(args)...)) return;
        }
        else
        {
            if (!newRes.load(std::forward<Args>(args)...)) return;
        }

        manager.replace(id, std::move(newRes));
    }

    template <typename T>
    auto get(const std::string& id) { return getManager<T>().getHandle(id); }

    template <typename T, typename HandleType>
    T* resolve(HandleType handle) { return getManager<T>().resolve(handle); }

    template <typename T, typename HandleType>
    void release(HandleType handle) { getManager<T>().release(handle); }

    void clear()
    {
        textures_.clear();
        fonts_.clear();
        sounds_.clear();
    }

private:
    Renderer& renderer_;
    AudioSystem& audio_;

    std::string basePath_;

    ResourceManager<Texture, TextureHandle> textures_;
    ResourceManager<Font, FontHandle> fonts_;
    ResourceManager<Sound, SoundHandle> sounds_;

    template <typename T>
    auto& getManager();
};

template <> inline auto& AssetManager::getManager<Texture>() { return textures_; }
template <> inline auto& AssetManager::getManager<Font>() { return fonts_; }
template <> inline auto& AssetManager::getManager<Sound>() { return sounds_; }

} // namespace dagger

#endif // DAGGER_ASSETS_ASSET_MANAGER_H
