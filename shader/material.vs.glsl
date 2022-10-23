#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexture;

out vec3 position;
out vec3 normal;
out vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    position = vec3(model * vec4(inPosition, 1.0));
    normal = mat3(transpose(inverse(model))) * inNormal;
    textureCoordinate = inTexture;

    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}