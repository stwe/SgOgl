#version 330

// terrain_with_lighting/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

// Out

out vec3 vPosition;
out vec3 vNormal;
out vec2 vUvOrig;
out vec2 vUv;

// Uniforms

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;

// Main

void main()
{
    vPosition = vec3(modelMatrix * vec4(aPosition, 1.0));
    vNormal = mat3(transpose(inverse(modelMatrix))) * aNormal;

    gl_Position = vpMatrix * vec4(vPosition, 1.0);

    vUvOrig = aUv;
    vUv = aUv * 80;
}
