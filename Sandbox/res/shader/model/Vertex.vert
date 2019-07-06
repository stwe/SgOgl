#version 330

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

// Out

out vec2 texCoord;

// Uniforms

uniform mat4 transform;

// Main

void main()
{
    gl_Position = transform * vec4(aPosition, 1.0);

    texCoord = aUv;
}
