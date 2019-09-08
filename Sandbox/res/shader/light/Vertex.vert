#version 330

// light/Vertex.vert

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

// Uniforms

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Main

void main()
{
    vPosition = vec3(model * vec4(aPosition, 1.0));
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vUv = aUv;

    gl_Position = projection * view * vec4(vPosition, 1.0);
}
