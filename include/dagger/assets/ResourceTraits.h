#ifndef DAGGER_ASSETS_RESOURCE_TRAITS_H
#define DAGGER_ASSETS_RESOURCE_TRAITS_H

namespace dagger
{

class Texture;
class Sound;
class Music;

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

template <>
struct ResourceTraits<Music>
{
    static constexpr bool needsRenderer = false;
    static constexpr bool needsMixer = true;
};

} // namespace dagger

#endif // DAGGER_ASSETS_RESOURCE_TRAITS_H
