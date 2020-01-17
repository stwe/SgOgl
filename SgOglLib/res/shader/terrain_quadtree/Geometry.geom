#version 430

// terrain_quadtree/Geometry.geom

// In

layout(triangles) in;

// Out

layout(line_strip, max_vertices = 4) out;

// Uniforms

uniform mat4 viewProjectionMatrix;

// Main

void main()
{
    for (int i = 0; i < gl_in.length(); ++i)
    {
        vec4 position = gl_in[i].gl_Position;
        gl_Position = viewProjectionMatrix * position;
        EmitVertex();
    }

    vec4 position = gl_in[0].gl_Position;
    gl_Position = viewProjectionMatrix * position;
    EmitVertex();

    EndPrimitive();
}
