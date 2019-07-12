#version 330

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 gsNormal[];

const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gsNormal[index], 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

// Main

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}
