#version 330

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

// Out

out vec3 ourColor;
out vec2 texCoord;

// Uniforms

uniform mat4 transform;

// Main

void main()
{
    gl_Position = transform * vec4(aPosition, 1.0);

    ourColor = aColor;
    texCoord = aUv;
}
