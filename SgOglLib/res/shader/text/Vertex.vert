#version 330

// text/Vertex.vert

// In

layout (location = 0) in vec4 aVertex; // <vec2 pos, vec2 tex>

// Out

out vec2 vUv;

// Uniforms

uniform mat4 projectionMatrix;

// Main

void main()
{
    gl_Position = projectionMatrix * vec4(aVertex.xy, 0.0, 1.0);
    vUv = aVertex.zw;
}
