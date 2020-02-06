#version 430

// terrain_quadtree/Fragment.frag

// In

in vec2 mapCoord_FS;
in vec3 worldPosition_FS;

// Out

out vec4 fragColor;

// Types

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuseIntensity;
    vec3 specularIntensity;
};

// Uniforms

uniform sampler2D normalmap;
uniform sampler2D splatmap;

uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

uniform vec3 ambientIntensity;
uniform DirectionalLight directionalLight;

// Function

vec4 CalcDirectionalLight(vec3 normal)
{
    vec3 lightDir = normalize(-directionalLight.direction);
    float diffuseFactor = max(0.0, dot(normal, lightDir));

    return diffuseFactor * vec4(directionalLight.diffuseIntensity, 1.0);
}

// Main

void main()
{
    vec3 normal = normalize(texture(normalmap, mapCoord_FS).rgb);
    vec4 blendValues = texture(splatmap, mapCoord_FS);

    vec4 sand = texture(sand, mapCoord_FS * 48.0);
    vec4 grass = texture(grass, mapCoord_FS * 100.0);
    vec4 rock = texture(rock, mapCoord_FS * 24.0);
    vec4 snow = texture(snow, mapCoord_FS * 48.0);

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    if (worldPosition_FS.y < 300.0)
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

    vec4 ambient = vec4(ambientIntensity, 1.0) * color;
    vec4 diffuse = CalcDirectionalLight(normal) * color;

    fragColor = ambient + diffuse;
    //fragColor = blendValues;
}
