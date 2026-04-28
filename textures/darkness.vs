#version 330

// Raylib default attribute locations
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexColor;

// Outputs to fragment shader
out vec3 worldPos;
out vec4 fragColor;

// Raylib uniforms
uniform mat4 mvp;
uniform mat4 matModel;

void main()
{
    // Compute world position
    worldPos = vec3(matModel * vec4(vertexPosition, 1.0));

    // Pass vertex color
    fragColor = vertexColor;

    // Standard transform
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
