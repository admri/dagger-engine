#ifndef DAGGER_ASSETS_RESOURCE_HANDLE_H
#define DAGGER_ASSETS_RESOURCE_HANDLE_H

#include <nlohmann/json.hpp>

#include <cstdint>

namespace dagger
{

class Texture;
class Font;
class Sound;
struct Prefab;

constexpr uint32_t INVALID_INDEX = 0xFFFFFFFF;

template <typename T>
struct ResourceHandle
{
    uint32_t index = INVALID_INDEX;
    uint32_t generation = 0;

    bool isValid() const { return index != INVALID_INDEX; }
};

using TextureHandle = ResourceHandle<Texture>;
using FontHandle = ResourceHandle<Font>;
using SoundHandle = ResourceHandle<Sound>;
using PrefabHandle = ResourceHandle<Prefab>;

} // namespace dagger

#endif // DAGGER_ASSETS_RESOURCE_HANDLE_H
