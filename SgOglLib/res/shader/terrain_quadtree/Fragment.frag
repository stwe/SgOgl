#version 430

// terrain_quadtree/Fragment.frag

// In

in vec2 mapCoord_FS;
in vec3 viewPosition_FS;
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
uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[2]; // max 2 directional lights

// Function

vec3 CalcDirectionalLight(DirectionalLight t_directionalLight, vec3 t_normal)
{
    vec3 lightDir = normalize(-t_directionalLight.direction);
    float diffuseFactor = max(0.0, dot(t_normal, lightDir));

    return diffuseFactor * t_directionalLight.diffuseIntensity;
}

// Fog

const vec3 fogColor = vec3(0.5, 0.6, 0.7);
const float fogDensity = 0.00025; // 0.0 ... 0.1

vec4 Fog(vec4 t_color)
{
    float fogFactor = 1.0 / exp(length(viewPosition_FS.xyz) * fogDensity);
    fogFactor = clamp(fogFactor, 0.0, 1.0 );

    vec3 result = mix(fogColor, t_color.xyz, fogFactor);

    return vec4(result.xyz, t_color.w);
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

    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < numDirectionalLights; ++i)
    {
        diffuse += CalcDirectionalLight(directionalLights[i], normal);
    }

    vec4 lightColor = ambient + vec4(diffuse, 1.0);

    fragColor = Fog(lightColor);
}
