#version 330

uniform vec3 darkPos;
uniform float radius;

in vec3 worldPos;
in vec4 fragColor;

out vec4 finalColor;

void main()
{
    float dist = length(worldPos - darkPos);
    float alpha = 1.0 - smoothstep(0.0, radius, dist);

    finalColor = vec4(0.0, 0.0, 0.0, alpha);
}
