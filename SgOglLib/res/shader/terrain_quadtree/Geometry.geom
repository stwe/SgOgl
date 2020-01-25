#version 430

// terrain_quadtree/Geometry.geom

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// In

in vec2 mapCoord_GS[];
in float height_GS[];

// Out

out vec2 mapCoord_FS;
out float height_FS;

// Uniforms

uniform mat4 viewProjectionMatrix;

// Main

void main()
{
    for (int i = 0; i < gl_in.length(); ++i)
    {
        vec4 position = gl_in[i].gl_Position;
        gl_Position = viewProjectionMatrix * position;
        mapCoord_FS = mapCoord_GS[i];
        height_FS = height_GS[i];
        EmitVertex();
    }

    EndPrimitive();
}
