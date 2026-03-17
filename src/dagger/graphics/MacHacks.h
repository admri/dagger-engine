#ifndef DAGGER_GRAPHICS_MAC_HACKS_H
#define DAGGER_GRAPHICS_MAC_HACKS_H

struct SDL_Renderer;

namespace dagger
{

void ApplyMetalOptimizations(SDL_Renderer* renderer);

} // namespace dagger

#endif // DAGGER_GRAPHICS_MAC_HACKS_H
