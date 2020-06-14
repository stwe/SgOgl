#version 330

// particle_system/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;

// Out

// Uniforms

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

// Main

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition.x, aPosition.y, 0.0, 1.0);
}
