#version 430

// terrain_quadtree/TessEval.tese

layout(quads, fractional_odd_spacing, cw) in;

// In

in vec2 mapCoord_TE[];

// Out

out vec2 mapCoord_GS;

// Uniforms

uniform sampler2D heightmap;
uniform float scaleXz;
uniform float scaleY;

// Function

vec4 worldPosition;
vec2 mapCoord;

void SetHeight()
{
    float height = texture(heightmap, mapCoord).r;
    height *= scaleY;

    worldPosition.y = height;
}

void ZeroHeightAtTheEdges()
{
    float hScaleXz = scaleXz * 0.5 - 16.0;
    if (worldPosition.x < -hScaleXz || worldPosition.x > hScaleXz)
    {
        worldPosition.y = 0.0;
    }

    if (worldPosition.z < -hScaleXz || worldPosition.z > hScaleXz)
    {
        worldPosition.y = 0.0;
    }
}

// Main

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    worldPosition =
        ((1 - u) * (1 - v) * gl_in[12].gl_Position +
        u * (1 - v) * gl_in[0].gl_Position +
        u * v * gl_in[3].gl_Position +
        (1 - u) * v * gl_in[15].gl_Position);

    mapCoord =
        ((1 - u) * (1 - v) * mapCoord_TE[12] +
        u * (1 - v) * mapCoord_TE[0] +
        u * v * mapCoord_TE[3] +
        (1 - u) * v * mapCoord_TE[15]);

    SetHeight();
    ZeroHeightAtTheEdges();

    mapCoord_GS = mapCoord;

    gl_Position = worldPosition;
}
