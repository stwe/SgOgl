#version 430

// terrain_quadtree/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

// Uniforms

uniform mat4 localMatrix;
uniform mat4 worldMatrix;

// Main

void main()
{
    vec2 localPosition = (localMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0)).xz;

    gl_Position = worldMatrix * vec4(localPosition.x, 0.0, localPosition.y, 1.0);
}
