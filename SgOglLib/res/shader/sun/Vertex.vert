#version 400

// sun/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec2 vUv;

// Uniforms

uniform mat4 mvpMatrix;

// Main

void main()
{
    gl_Position = mvpMatrix * vec4(aPosition, 0.0, 1.0);

    vUv = aPosition + vec2(0.5, 0.5);
    vUv.y = 1.0 - vUv.y;
}
