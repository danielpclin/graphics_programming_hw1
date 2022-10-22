#version 410 core

in vec3 position;
in vec3 normal;

layout (location = 0) out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void)
{
    // ambient
    float ambientStrength = 0.1;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPos - position);
    float diff = max(dot(norm, lightDirection), 0.0);

    fragColor = vec4((diff + ambientStrength) * lightColor * objectColor, 1.0);
}