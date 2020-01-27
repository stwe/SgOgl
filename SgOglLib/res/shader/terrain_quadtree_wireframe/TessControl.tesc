#version 430

// terrain_quadtree_wireframe/TessControl.tesc

layout(vertices = 16) out;

// In

in vec2 mapCoord_TC[];

// Out

out vec2 mapCoord_TE[];

// Uniforms

uniform int tessellationFactor;
uniform float tessellationSlope;
uniform float tessellationShift;
uniform vec3 cameraPosition;
uniform float tessellationEnabled;

// Const

const int AB = 2;
const int BC = 3;
const int CD = 0;
const int DA = 1;

// Function

float LodFactor(float t_dist)
{
    return max(0.0, tessellationFactor / pow(t_dist, tessellationSlope) + tessellationShift);
}

void SetTessLevel()
{
        vec3 abMid = vec3(gl_in[0].gl_Position + gl_in[3].gl_Position) / 2.0;
        vec3 bcMid = vec3(gl_in[3].gl_Position + gl_in[15].gl_Position) / 2.0;
        vec3 cdMid = vec3(gl_in[15].gl_Position + gl_in[12].gl_Position) / 2.0;
        vec3 daMid = vec3(gl_in[12].gl_Position + gl_in[0].gl_Position) / 2.0;
        
        float distanceAB = distance(abMid, cameraPosition);
        float distanceBC = distance(bcMid, cameraPosition);
        float distanceCD = distance(cdMid, cameraPosition);
        float distanceDA = distance(daMid, cameraPosition);
        
        gl_TessLevelOuter[AB] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceAB));
        gl_TessLevelOuter[BC] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceBC));
        gl_TessLevelOuter[CD] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceCD));
        gl_TessLevelOuter[DA] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceDA));
        
        gl_TessLevelInner[0] = (gl_TessLevelOuter[BC] + gl_TessLevelOuter[DA]) / 4;
        gl_TessLevelInner[1] = (gl_TessLevelOuter[AB] + gl_TessLevelOuter[CD]) / 4;
}

// Main

void main()
{
    if (gl_InvocationID == 0)
    {
        if (tessellationEnabled > 0.5)
        {
            SetTessLevel();
        }
        else
        {
            gl_TessLevelOuter[AB] = 1;
            gl_TessLevelOuter[BC] = 1;
            gl_TessLevelOuter[CD] = 1;
            gl_TessLevelOuter[DA] = 1;

            gl_TessLevelInner[0] = 1;
            gl_TessLevelInner[1] = 1;
        }
    }

    mapCoord_TE[gl_InvocationID] = mapCoord_TC[gl_InvocationID];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
