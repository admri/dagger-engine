#ifndef DAGGER_GRAPHICS_VIEWPORT_H
#define DAGGER_GRAPHICS_VIEWPORT_H

namespace dagger
{

struct Viewport
{
    float camX = 0.0f;
    float camY = 0.0f;
    int windowWidth = 0;
    int windowHeight = 0;
    float alpha = 0.0f;
};
    
} // namespace dagger

#endif // DAGGER_GRAPHICS_VIEWPORT_H
