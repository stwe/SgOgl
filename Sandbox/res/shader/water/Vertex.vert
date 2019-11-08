#version 330

// water/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec4 vClipSpace;
out vec2 vUv;
out vec3 vToCameraVector;
out vec3 vFromLightVector;

// Uniforms

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

// Main

const float tiling = 4.0;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0);
    vClipSpace = vpMatrix * worldPosition;
    gl_Position = vClipSpace;

    vUv = vec2(aPosition.x / 2.0 + 0.5, aPosition.y / 2.0 + 0.5) * tiling;

    vToCameraVector = cameraPosition - worldPosition.xyz;
    vFromLightVector = worldPosition.xyz - lightPosition;
}
