#ifndef RESOURCE_HANDLE_H
#define RESOURCE_HANDLE_H

#include <cstdint>

namespace engine
{

struct ResourceHandle
{
    uint32_t index = 0xFFFFFFFF;
    uint32_t generation = 0;

    bool isValid() const { return index != 0xFFFFFFFF; }
};

using TextureHandle = ResourceHandle;
using FontHandle = ResourceHandle;
using SoundHandle = ResourceHandle;

} // namespace engine

#endif // RESOURCE_HANDLE_H
