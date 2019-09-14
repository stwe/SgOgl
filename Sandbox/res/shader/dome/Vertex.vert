#version 330

// dome/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;

// Out

out vec3 worldPosition;

// Uniforms

uniform mat4 mvpMatrix;
uniform mat4 worldMatrix;

// Main

void main()
{
    gl_Position = mvpMatrix * vec4(aPosition, 1.0);
    worldPosition = (worldMatrix * vec4(aPosition, 1.0)).xyz;
}
