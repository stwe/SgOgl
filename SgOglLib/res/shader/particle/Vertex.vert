#version 330

// particle/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

// Uniforms

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

// Main

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition, 0.0, 1.0);
}
