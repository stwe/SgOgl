#version 330

// skybox/Vertex.vert

// In

layout (location = 0) in vec3 aPosition;

// Out

out vec3 vUv;

// Uniforms

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

// Main

void main()
{
    vUv = aPosition;
    vec4 position = projectionMatrix * viewMatrix * vec4(aPosition, 1.0);
    gl_Position = position.xyww;
}
