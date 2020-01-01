#version 330

// model/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

// Out

out vec3 vPosition;
out vec3 vNormal;
out vec2 vUv;
out mat3 vTbnMatrix;

// Uniforms

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform vec4 plane;

// Function

mat3 GetTbnMatrix()
{
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    return transpose(mat3(T, B, N));
}

// Main

void main()
{
    gl_Position = mvpMatrix * vec4(aPosition, 1.0);

    vec4 worldPosition = modelMatrix * vec4(aPosition, 1.0);
    gl_ClipDistance[0] = dot(worldPosition, plane);

    vPosition = vec3(worldPosition);
    vNormal = mat3(transpose(inverse(modelMatrix))) * aNormal;
    vUv = aUv;
    vTbnMatrix = GetTbnMatrix();
}
