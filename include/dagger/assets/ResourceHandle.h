#ifndef DAGGER_ASSETS_HANDLE_H
#define DAGGER_ASSETS_HANDLE_H

#include <cstdint>

namespace dagger
{

class Texture;
class Font;
class Sound;

template <typename T>
struct Handle
{
    uint32_t index = 0xFFFFFFFF;
    uint32_t generation = 0;

    bool isValid() const { return index != 0xFFFFFFFF; }
};

using TextureHandle = Handle<Texture>;
using FontHandle = Handle<Font>;
using SoundHandle = Handle<Sound>;

} // namespace dagger

#endif // DAGGER_ASSETS_HANDLE_H
