#version 430

// terrain_quadtree/TessEval.tese

// In

layout(quads, fractional_odd_spacing, cw) in;

// Main

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 worldPosition =
        ((1 - u) * (1 - v) * gl_in[12].gl_Position +
        u * (1 - v) * gl_in[0].gl_Position +
        u * v * gl_in[3].gl_Position +
        (1 - u) * v * gl_in[15].gl_Position);

    gl_Position = worldPosition;
}
