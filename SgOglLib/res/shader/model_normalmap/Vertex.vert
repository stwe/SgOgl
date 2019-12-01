#version 330

// model_normalmap/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

// Out

out vec2 vUv;

out vec3 vTangentFragmentPosition;
out vec3 vTangentDirectionalLightDirection;
out vec3 vTangentPointLightPosition;
out vec3 vTangentCameraPosition;

// Uniforms

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform vec4 plane;

uniform vec3 directionalLightDirection;
uniform vec3 pointLightPosition;
uniform vec3 cameraPosition;

// Main

void main()
{
    gl_Position = mvpMatrix * vec4(aPosition, 1.0);

    vec4 worldPosition = modelMatrix * vec4(aPosition, 1.0);
    gl_ClipDistance[0] = dot(worldPosition, plane);

    vUv = aUv;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));
    vTangentFragmentPosition  = TBN * vec3(worldPosition);
    vTangentDirectionalLightDirection = TBN * directionalLightDirection;
    vTangentPointLightPosition = TBN * pointLightPosition;
    vTangentCameraPosition  = TBN * cameraPosition;
}
