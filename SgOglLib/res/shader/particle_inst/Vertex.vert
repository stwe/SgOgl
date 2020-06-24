#version 330

// particle_inst/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// In  - per instance vars

layout (location = 1) in mat4 modelViewMatrix;
layout (location = 5) in vec4 texOffsets;
layout (location = 6) in float blendFactor;

// Out

out vec2 vUvCurrent;
out vec2 vUvNext;
out float vBlend;

// Uniforms

uniform mat4 projectionMatrix;
uniform int textureRows;

// Main

void main()
{
    vec2 uv = aPosition + vec2(0.5, 0.5);
    uv.y = 1.0 - uv.y;

    uv /= textureRows;

    vUvCurrent = uv + texOffsets.xy;
    vUvNext = uv + texOffsets.zw;
    vBlend = blendFactor;

    gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition, 0.0, 1.0);
}
