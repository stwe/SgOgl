#version 330

// lighting_pass/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUv;

// Out

out vec2 vUv;

// Main

void main()
{
    vUv = aUv;
    gl_Position = vec4(aPosition, 1.0);
}
