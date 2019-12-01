#version 330

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

// Out

out vec3 vNormal;

// Uniforms

uniform mat4 transform;

// Main

void main()
{
    gl_Position = transform * vec4(aPosition, 1.0);
    vNormal = aNormal;
}
