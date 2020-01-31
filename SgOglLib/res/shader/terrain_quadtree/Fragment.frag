#version 430

// terrain_quadtree/Fragment.frag

// In

in vec2 mapCoord_FS;
in float height_FS;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D normalmap;
uniform sampler2D splatmap;

uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

// Function

float Diffuse(vec3 t_direction, vec3 t_normal, float t_intensity)
{
    return max(0.3, dot(t_normal, -t_direction * t_intensity));
}

// Const

const vec3 lightDirection = vec3(0.1f, -1.0f, 0.1);
const float intensity = 1.2;

// Main

void main()
{
    vec3 normal = normalize(texture(normalmap, mapCoord_FS).rgb);
    vec4 blendValues = texture(splatmap, mapCoord_FS);

    // transform normal vector to range [-1, 1]
    //vec3 normalTrans = normalize(normal * 2.0 - 1.0);

    vec4 sand = texture(sand, mapCoord_FS * 100.0);
    vec4 grass = texture(grass, mapCoord_FS * 100.0);
    vec4 rock = texture(rock, mapCoord_FS * 24.0);
    vec4 snow = texture(snow, mapCoord_FS * 24.0);

    float diff = Diffuse(lightDirection, normal, intensity);

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    // height_FS

    if (height_FS < 300.0)
    {
        color = blendValues.r * sand;
    }
    else
    {
        color = blendValues.r * snow;
    }

    color += blendValues.g * grass;
    color += blendValues.b * rock;
    color += blendValues.a * snow;

    fragColor = color * diff;
    //fragColor = blendValues;
}
