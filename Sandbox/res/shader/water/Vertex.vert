#version 330

// water/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

// Out

out vec4 vClipSpace;
out vec2 vUv;
out vec3 vToCameraVector;
out vec3 vFromLightVector;

// Uniforms

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

// Main

const float tiling = 4.0;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(aPosition, 1.0);
    vClipSpace = mvpMatrix * vec4(aPosition, 1.0);
    gl_Position = vClipSpace;

    // use position.z!
    vUv = vec2(aPosition.x / 2.0 + 0.5, aPosition.z / 2.0 + 0.5) * tiling;

    vToCameraVector = cameraPosition - worldPosition.xyz;
    vFromLightVector = worldPosition.xyz - lightPosition;
}
