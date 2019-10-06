#version 330

// particle_anim/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// In  - per instance vars

layout (location = 1) in mat4 modelViewMatrix;
layout (location = 5) in vec4 texOffsets;
layout (location = 6) in float blendFactor;

// Out

out vec2 vUv1;
out vec2 vUv2;
out float vBlend;

// Uniforms

uniform mat4 projectionMatrix;
uniform float numberOfRows;

// Main

void main()
{
    vec2 textureCoords = aPosition + vec2(0.5, 0.5);
    textureCoords.y = 1.0 - textureCoords.y;
    textureCoords /= numberOfRows;

    vUv1 = textureCoords + texOffsets.xy;
    vUv2 = textureCoords + texOffsets.zw;

    vBlend = blendFactor;

    gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition, 0.0, 1.0);
}
