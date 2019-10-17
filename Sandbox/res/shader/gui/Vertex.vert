#version 400

// gui/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec2 vUv;

// Uniforms

uniform mat4 transformationMatrix;

// Main

void main()
{
    gl_Position = transformationMatrix * vec4(aPosition, 0.0, 1.0);
    vUv = vec2((aPosition.x + 1.0) / 2.0, 1.0 - (aPosition.y + 1.0) / 2.0);
}
