#version 450

layout(location = 0) in vec4 inColor;     
layout(location = 1) in vec2 inTexCoord;  

layout(location = 0) out vec4 outColor;

// Map defining the walls
layout(set = 2, binding = 0) uniform sampler2D occlusionMap;

layout(set = 3, binding = 0, std140) uniform LightData {
    float lightX;
    float lightY;
    float lightRadius;
    float aspectRatio;
} ubo;

void main() 
{
    vec2 pos = inTexCoord; // Pixel position
    vec2 lightPos = vec2(ubo.lightX, ubo.lightY); // Light source position
    vec2 aspectVec = vec2(ubo.aspectRatio, 1.0); // Screen aspect ratio

    float dist = length((lightPos - pos) * aspectVec); // Distance from light source
    float attenuation = smoothstep(ubo.lightRadius, ubo.lightRadius * 0.05, dist); // Smooth light fading

    // Skip calculations if pixel is farther away than the light can reach
    if (dist > ubo.lightRadius)
    {
        outColor = vec4(0.0, 0.0, 0.0, 0.65); // Return dimmed black filter
        return;
    }

    // Check if pixel is wall
    float targetIsWall = texture(occlusionMap, pos).r;

    // Algorithm step count, and single-step direction and size
    int steps = clamp(int(dist * 800.0), 30, 300);
    vec2 dir = pos - lightPos;
    vec2 stepVec = dir / float(steps);

    float shadow = 1.0; // Pixel is lit up by default

    // Cast a ray from the pixel to the light source and trace it by steps
    for (int i = 0; i < steps; ++i) 
    {
        // Calculate current ray position
        vec2 samplePos = lightPos + stepVec * float(i);

        // Check for obstacle at ray position
        float obstacle = texture(occlusionMap, samplePos).r; 
        
        // Obstacle hit
        if (obstacle > 0.5)
        {
            if (targetIsWall > 0.5)
            {
                // Keep pixel fully lit if it's also wall 
                shadow = 1.0;
            }
            else
            {
                // Throw a shadow if it's floor blocked by a wall
                shadow = 0.0;
            }

            // Stop the ray after we have hit an obstacle
            break;
        }
    }

    // Base darkness minus the light that reaches the pixel
    float finalAlpha = 0.65 - (attenuation * shadow * 0.65);

    // Set the output to black filter with applied alpha (clamped final alpha)
    outColor = vec4(0.0, 0.0, 0.0, clamp(finalAlpha, 0.0, 0.65));
}