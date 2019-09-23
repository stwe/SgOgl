#version 330

// instancing/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
layout (location = 5) in mat4 aInstanceMatrix;

// Out

out vec2 vUv;

// Uniforms

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Main

void main()
{
    vec3 position = vec3(aInstanceMatrix * vec4(aPosition, 1.0));
    vUv = aUv;

    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}
