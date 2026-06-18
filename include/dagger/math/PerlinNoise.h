#ifndef DAGGER_MATH_PERLIN_NOISE_H
#define DAGGER_MATH_PERLIN_NOISE_H

#include <cmath>
#include <cstdint>
#include <numbers>
#include <utility>

namespace dagger
{

class PerlinNoise
{
public:
    explicit PerlinNoise(uint64_t seed) : seed_(seed) {}

    // Generate base 2D noise for a specific point
    float noise(float x, float y) const
    {
        // Grid cell coordinates (top-left corner)
        int x0 = static_cast<int>(std::floor(x));
        int y0 = static_cast<int>(std::floor(y));

        // Grid cell coordinates (bottom-right corner)
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        // Fractional distance of the point inside the grid cell (0.0 to 1.0)
        float sx = x - static_cast<float>(x0);
        float sy = y - static_cast<float>(y0);

        // Influence of the random gradient at each corner
        float n0 = dotGridGradient(x0, y0, x, y);
        float n1 = dotGridGradient(x1, y0, x, y);
        float n2 = dotGridGradient(x0, y1, x, y);
        float n3 = dotGridGradient(x1, y1, x, y);

        // Blend the top two and bottom two corners respectively (horizontally)
        float ix0 = interpolate(n0, n1, sx);
        float ix1 = interpolate(n2, n3, sx);

        // Blend the two horizontal results vertically
        return interpolate(ix0, ix1, sy);
    }

    // Fractal Brownian Motion
    // Layers multiple grids for natural detail
    float fractal(float x, float y, int octaves, float persistence, float scale) const
    {
        float total = 0.0f;
        float frequency = scale; // 
        float amplitude = 1.0f;  //
        float maxValue = 0.0f;   // 

        for (int i = 0; i < octaves; ++i)
        {
            total += noise(x * frequency, y * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence; // Lower amplitude for smaller details
            frequency *= 2.0f;        // Tighter grid for finer details
        }

        // Normalize back to 0.0-1.0
        return (total / maxValue) * 0.5f + 0.5f;
    }

private:
    uint64_t seed_;

    // Deterministic hash (MurmurHash3 fmix64 algorithm)
    uint64_t hash(int x, int y) const
    {
        uint64_t h = (static_cast<uint64_t>(static_cast<uint32_t>(x)) << 32) | static_cast<uint32_t>(y);
        h ^= seed_; // Mix in the seed
        h ^= h >> 33; h *= 0xff51afd7ed558ccd; // Mix higher bits with lower bits,
        h ^= h >> 33; h *= 0xc4ceb9fe1a85ec53; // then multiply by a prime number
        h ^= h >> 33; // Do 3 mixing passes
        return h;
    }

    float dotGridGradient(int ix, int iy, float x, float y) const
    {
        // Get a "random" angle in radians
        uint64_t h = hash(ix, iy);
        float randomAngle = static_cast<float>(h % 360) * (std::numbers::pi_v<float> / 180.0f);

        // Convert the angle into a 2D unit vector (the gradient arrow)
        float gradientX = std::cos(randomAngle);
        float gradientY = std::sin(randomAngle);

        // Calculate the distance vector from this grid corner to our specific pixel
        float dx = x - static_cast<float>(ix);
        float dy = y - static_cast<float>(iy);

        // Calculate the dot product of the two vectors
        return (dx * gradientX + dy * gradientY);
    }

    // Blends two values smoothly to prevent blocky artifacts
    float interpolate(float a, float b, float weight) const
    {
        // The smoothstep equation: 3x^2 - 2x^3
        float smoothWeight = weight * weight * (3.0f - 2.0f * weight);
        
        // Standard linear interpolation using the smoothened weight
        return a + smoothWeight * (b - a);
    }
};

} // namespace dagger

#endif // DAGGER_MATH_PERLIN_NOISE_H
