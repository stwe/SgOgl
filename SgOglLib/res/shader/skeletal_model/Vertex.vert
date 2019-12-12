#version 330

// skeletal_model/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
layout (location = 5) in ivec4 aBoneIds;
layout (location = 6) in vec4 aWeights;

// Out

out vec3 vPosition;
out vec3 vNormal;
out vec2 vUv;

// Uniforms

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform vec4 plane;
uniform mat4 bones[100];

// Main

void main()
{
    mat4 bone_transform = bones[aBoneIds[0]] * aWeights[0];
    bone_transform += bones[aBoneIds[1]] * aWeights[1];
    bone_transform += bones[aBoneIds[2]] * aWeights[2];
    bone_transform += bones[aBoneIds[3]] * aWeights[3];

    vec4 boned_position = bone_transform * vec4(aPosition, 1.0);

    gl_Position = mvpMatrix * boned_position;

    vPosition = vec3(modelMatrix * boned_position);
    vNormal = mat3(transpose(inverse(modelMatrix))) * aNormal;
    vUv = aUv;
}
