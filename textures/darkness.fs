#version 330

// 1. Uniforms (values your C++ code sends in)
uniform vec3 darkPos;     // center of the darkness
uniform float radius;     // how big the darkness is

// 2. Inputs from Raylib
in vec3 fragPosition;     // world position of this pixel
in vec4 fragColor;        // base color

// 3. Output
out vec4 finalColor;

void main()
{
    // 4. Compute distance from center
    float dist = length(fragPosition - darkPos);

    // 5. Map distance to opacity
    float alpha = 1.0 - smoothstep(0.0, radius, dist);

    // 6. Output final color
    finalColor = vec4(0.0, 0.0, 0.0, alpha);
}
