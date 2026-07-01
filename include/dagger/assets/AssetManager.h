#ifndef DAGGER_ASSETS_ASSET_MANAGER_H
#define DAGGER_ASSETS_ASSET_MANAGER_H

#include <dagger/assets/Font.h>
#include <dagger/assets/Prefab.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/assets/ResourceManager.h>
#include <dagger/assets/ResourceTraits.h>
#include <dagger/audio/AudioManager.h>
#include <dagger/audio/AudioResource.h>
#include <dagger/graphics/Texture.h>
#include <dagger/utils/StringHash.h>

#include <memory>
#include <string>
#include <utility>

namespace dagger
{

class Renderer;

/// @brief Central registry for loading, storing, and retrieving game assets.
/// Manages the lifecycle of resources (Textures, Fonts, Sounds, etc.) and issues safe ResourceHandles.
class AssetManager
{
public:
    AssetManager(Renderer& renderer, AudioManager& audio);
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    /// @brief Converts a relative asset path to an absolute path based on the engine's root directory.
    std::string getAbsolutePath(const std::string& relativePath) const
    {
        return basePath_ + relativePath;
    }

    /// @brief Loads an asset from disk into memory and assigns it a unique string ID.
    /// @tparam T The resource type (e.g., Texture, Sound).
    /// @param id The unique string identifier for this asset (e.g., "player_tex").
    /// @param filePath The relative path to the asset file.
    /// @return A safe generational handle to the loaded resource, or an invalid handle on failure.
    template <typename T, typename... Args>
    auto load(StringHash id, const std::string& filePath = "", Args&&... args)
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

    /// @brief Replaces an existing asset in memory without invalidating existing handles.
    /// @param id The string identifier of the asset to replace.
    template <typename T, typename... Args>
    void replace(StringHash id, Args&&... args)
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

    /// @brief Retrieves an existing asset handle by its string ID.
    /// @return The resource handle, or an invalid handle if not found.
    template <typename T>
    auto get(StringHash id) { return getManager<T>().getHandle(id); }

    /// @brief Resolves a generational handle into a raw pointer for immediate use.
    /// @param handle The handle to resolve.
    /// @return Pointer to the asset, or nullptr if the asset was freed or the handle is invalid.
    template <typename T, typename HandleType>
    T* resolve(HandleType handle) { return getManager<T>().resolve(handle); }

    /// @brief Unloads an asset from memory and invalidates its handles.
    template <typename T, typename HandleType>
    void release(HandleType handle) { getManager<T>().release(handle); }

    /// @brief Unloads all currently loaded assets and frees memory.
    void clear()
    {
        textures_.clear();
        fonts_.clear();
        sounds_.clear();
        music_.clear();
        prefabs_.clear();
    }

private:
    Renderer& renderer_;
    AudioManager& audio_;

    std::string basePath_;

    ResourceManager<Texture, TextureHandle> textures_;
    ResourceManager<Font, FontHandle> fonts_;
    ResourceManager<Sound, SoundHandle> sounds_;
    ResourceManager<Music, MusicHandle> music_;
    ResourceManager<Prefab, PrefabHandle> prefabs_;

    template <typename T>
    auto& getManager();
};

template <> inline auto& AssetManager::getManager<Texture>() { return textures_; }
template <> inline auto& AssetManager::getManager<Font>() { return fonts_; }
template <> inline auto& AssetManager::getManager<Sound>() { return sounds_; }
template <> inline auto& AssetManager::getManager<Music>() { return music_; }
template <> inline auto& AssetManager::getManager<Prefab>() { return prefabs_; }

} // namespace dagger

#endif // DAGGER_ASSETS_ASSET_MANAGER_H
