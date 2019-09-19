#version 330

// terrain/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

// Out

out vec2 vUvOrig;
out vec2 vUv;

// Uniforms

uniform mat4 transform;

// Main

void main()
{
    gl_Position = transform * vec4(aPosition, 1.0);

    vUvOrig = aUv;
    vUv = aUv * 40;
}
