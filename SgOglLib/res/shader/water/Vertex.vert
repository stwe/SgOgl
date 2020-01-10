#version 330

// water/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec3 vWorldPosition;
out vec4 vClipSpace;
out vec2 vUv;

// Uniforms

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;

// Main

const float tiling = 4.0;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0);

    vWorldPosition = worldPosition.xyz;
    vClipSpace = vpMatrix * worldPosition;
    vUv = vec2(aPosition.x / 2.0 + 0.5, aPosition.y / 2.0 + 0.5) * tiling;

    gl_Position = vClipSpace;
}
