#ifndef DAGGER_ASSETS_RESOURCE_HANDLE_H
#define DAGGER_ASSETS_RESOURCE_HANDLE_H

#include <cstdint>

namespace dagger
{

class Texture;
class Font;
class Sound;

constexpr uint32_t INVALID_INDEX = 0xFFFFFFFF;

template <typename T>
struct Handle
{
    uint32_t index = INVALID_INDEX;
    uint32_t generation = 0;

    bool isValid() const { return index != INVALID_INDEX; }
};

using TextureHandle = Handle<Texture>;
using FontHandle = Handle<Font>;
using SoundHandle = Handle<Sound>;

} // namespace dagger

#endif // DAGGER_ASSETS_RESOURCE_HANDLE_H
