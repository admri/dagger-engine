#ifndef DAGGER_ASSETS_RESOURCE_HANDLE_H
#define DAGGER_ASSETS_RESOURCE_HANDLE_H

#include <nlohmann/json.hpp>

#include <cstdint>

namespace dagger
{

class Texture;
class Font;
class Sound;
class Music;
struct Prefab;

constexpr uint32_t INVALID_INDEX = 0xFFFFFFFF;

/// @brief A safe, generational pointer replacement.
/// Prevents dangling pointers by ensuring the generation of the handle matches the resource in memory.
template <typename T>
struct ResourceHandle
{
    uint32_t index = INVALID_INDEX;
    uint32_t generation = 0;

    /// @brief Checks if the handle points to a valid memory index. 
    /// (Note: Does not guarantee the asset hasn't been freed; use AssetManager::resolve for that).
    bool isValid() const { return index != INVALID_INDEX; }
};

using TextureHandle = ResourceHandle<Texture>;
using FontHandle = ResourceHandle<Font>;
using SoundHandle = ResourceHandle<Sound>;
using MusicHandle = ResourceHandle<Music>;
using PrefabHandle = ResourceHandle<Prefab>;

} // namespace dagger

#endif // DAGGER_ASSETS_RESOURCE_HANDLE_H
