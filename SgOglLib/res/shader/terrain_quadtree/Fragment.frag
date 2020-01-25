#version 430

// terrain_quadtree/Fragment.frag

// In

in vec2 mapCoord_FS;
in float height_FS;

// Out

out vec4 fragColor;

// Uniforms

uniform vec3 color;

uniform sampler2D normalmap;

uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

// Function

float Diffuse(vec3 t_direction, vec3 t_normal, float t_intensity)
{
    return max(0.04, dot(t_normal, -(t_direction) * t_intensity));
}

vec3 GetColor(vec3 t_sand, vec3 t_grass, vec3 t_rock, vec3 t_snow, vec3 t_normal)
{
    vec3 heightColor;

    float cosV = abs(dot(t_normal, vec3(0.0, 1.0, 0.0)));
    float gc = 0.75; // 0.75
    float waterHeight = 10.0;
    float trans = 30.0; // 0.4
    float grassCoverage = pow(gc, 0.33);

    float tenPercentGrass = grassCoverage - grassCoverage * 0.2;
    float blendingCoeff = pow((cosV - tenPercentGrass) / (grassCoverage * 0.1), 4.0);

    // < 40
    if(height_FS <= waterHeight + trans)
    {
        heightColor = t_sand;
    } // <= 70
    else if(height_FS <= waterHeight + 2 * trans) // 10 + 60
    {                                       // 50 - 10 - 30 = 10 / 30
        heightColor = mix(t_sand, t_grass, (height_FS - waterHeight - trans) / trans);
    }
    else
    {
        heightColor = mix(t_grass, t_rock, cosV);
    }

    return heightColor;
}

// Const

const vec3 lightDirection = vec3(0.1f, -1.0f, 0.1);
const float intensity = 1.2;

// Main

void main()
{
    vec3 normal = normalize(texture(normalmap, mapCoord_FS).rgb);

    vec3 sand = texture(sand, mapCoord_FS * 10.0).rgb;
    vec3 grass = texture(grass, mapCoord_FS * 35.0).rgb;
    vec3 rock = texture(rock, mapCoord_FS * 35.0).rgb;
    vec3 snow = texture(snow, mapCoord_FS * 5.0).rgb;
    rock.r *= 1.1;

    float diff = Diffuse(lightDirection, normal, intensity);

    fragColor = vec4(GetColor(sand, grass, rock, snow, normal), 1.0) * diff;
}
