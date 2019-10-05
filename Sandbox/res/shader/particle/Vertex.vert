#version 330

// particle/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec2 vUv;

// Uniforms

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform float numberOfRows;
uniform vec2 offset;

// Main

void main()
{
    vUv = aPosition + vec2(0.5, 0.5);
    vUv.y = 1.0 - vUv.y;

    vUv = (vUv / numberOfRows) + offset;

    gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition, 0.0, 1.0);
}
