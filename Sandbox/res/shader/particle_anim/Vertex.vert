#version 330

// particle_anim/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec2 vUv1;
out vec2 vUv2;
out float vBlend;

// Uniforms

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform float numberOfRows;
uniform float blendFactor;
uniform vec2 offset1;
uniform vec2 offset2;

// Main

void main()
{
    vec2 textureCoords = aPosition + vec2(0.5, 0.5);
    textureCoords.y = 1.0 - textureCoords.y;
    textureCoords /= numberOfRows;

    vUv1 = textureCoords + offset1;
    vUv2 = textureCoords + offset2;

    vBlend = blendFactor;

    gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition, 0.0, 1.0);
}
