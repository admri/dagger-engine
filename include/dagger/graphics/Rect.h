#ifndef DAGGER_GRAPHICS_RECT_H
#define DAGGER_GRAPHICS_RECT_H

namespace dagger
{

/// @brief A basic 2D rectangle structure used for rendering destinations and source crops.
struct Rect
{
    float x;
    float y;
    float w;
    float h;
};

} // namespace dagger

#endif // DAGGER_GRAPHICS_RECT_H
