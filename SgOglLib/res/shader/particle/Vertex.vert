#version 330

// particle/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec2 vUvCurrent;
out vec2 vUvNext;
out float vBlend;

// Uniforms

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform vec2 texOffsetCurrent;
uniform vec2 texOffsetNext;
uniform int textureRows;
uniform float blendFactor;

// Main

void main()
{
    vec2 uv = aPosition + vec2(0.5, 0.5);
    uv.y = 1.0 - uv.y;

    uv /= textureRows;

    vUvCurrent = uv + texOffsetCurrent;
    vUvNext = uv + texOffsetNext;
    vBlend = blendFactor;

    gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition, 0.0, 1.0);
}
