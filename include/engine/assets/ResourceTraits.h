#ifndef RESOURCE_TRAITS_H
#define RESOURCE_TRAITS_H

namespace engine
{

class Texture;
class Sound;

template <typename T>
struct ResourceTraits
{
    static constexpr bool needsRenderer = false;
    static constexpr bool needsMixer = false;
};

template <>
struct ResourceTraits<Texture>
{
    static constexpr bool needsRenderer = true;
    static constexpr bool needsMixer = false;
};

template <>
struct ResourceTraits<Sound>
{
    static constexpr bool needsRenderer = false;
    static constexpr bool needsMixer = true;
};

}

#endif // RESOURCE_TRAITS_H
