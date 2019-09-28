#version 330

// particle/Vertex.vert

// In

layout (location = 0) in vec3 aSquareVertices;
layout (location = 1) in vec4 aXyzS;
layout (location = 2) in vec4 aColor;

// Out

out vec2 vUv;
out vec4 vColor;

// Uniforms

uniform vec3 cameraRightWorldspace;
uniform vec3 cameraUpWorldspace;
uniform mat4 vpMatrix;

// Main

void main()
{
    float particleSize = aXyzS.w; // because we encoded it this way.
    vec3 particleCenterWordspace = aXyzS.xyz;

    vec3 vertexPositionWorldspace = 
        particleCenterWordspace
        + cameraRightWorldspace * aSquareVertices.x * particleSize
        + cameraUpWorldspace * aSquareVertices.y * particleSize;

    // Output position of the vertex
    gl_Position = vpMatrix * vec4(vertexPositionWorldspace, 1.0f);

    // Uv of the vertex. No special space for this one.
    vUv = aSquareVertices.xy + vec2(0.5, 0.5);
    vColor = aColor;
}
