#version 330

// water/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

// Out

out vec4 clipSpace;

// Uniforms

uniform mat4 mvpMatrix;

// Main

void main()
{
    clipSpace = mvpMatrix * vec4(aPosition, 1.0);
    gl_Position = clipSpace;
}
