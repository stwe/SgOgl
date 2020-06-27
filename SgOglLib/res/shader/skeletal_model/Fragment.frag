#version 330

// skeletal_model/Fragment.frag

// In

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUv;

// Out

out vec4 fragColor;

// Types

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuseIntensity;
    vec3 specularIntensity;
};

struct PointLight
{
    vec3 position;
    vec3 ambientIntensity;
    vec3 diffuseIntensity;
    vec3 specularIntensity;
    float constant;
    float linear;
    float quadratic;
};

// Uniforms

uniform int numPointLights;
uniform int numDirectionalLights;

uniform vec3 ambientIntensity;

uniform PointLight pointLights[12];            // max 12 point lights
uniform DirectionalLight directionalLights[2]; // max 2 directional lights

uniform vec3 cameraPosition;

uniform vec3 diffuseColor;
uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;

uniform vec3 specularColor;
uniform float hasSpecularMap;
uniform sampler2D specularMap;

uniform float shininess;

// Global

vec4 diffuse;
vec4 specular;

// Function

void SetupColors()
{
    // get diffuse color
    diffuse = vec4(diffuseColor, 1.0);
    if (hasDiffuseMap > 0.5)
    {
        diffuse = texture(diffuseMap, vUv);
    }

    // get specular color
    specular = vec4(specularColor, 1.0);
    if (hasSpecularMap > 0.5)
    {
        specular = texture(specularMap, vUv);
    }
}

vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-directionalLight.direction);

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diff = directionalLight.diffuseIntensity * diffuseFactor * diffuse.rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 spec = directionalLight.specularIntensity * specularFactor * specular.rgb;

    // result
    return diff + spec;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // light direction
    vec3 lightDir = normalize(light.position - fragPos);

    // ambient
    vec3 ambient = light.ambientIntensity * diffuse.rgb;

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuseIntensity * diffuseFactor * diffuse.rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specularIntensity * specularFactor * specular.rgb;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // result
    return ambient + diffuse + specular;
}

// Main

void main()
{
    SetupColors();

    //if (diffuse.a < 0.5)
    //{
    //    discard;
    //}

    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(cameraPosition - vPosition);

    vec3 ambient = ambientIntensity * diffuse.rgb;

    vec3 result = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < numDirectionalLights; ++i)
    {
        result += CalcDirectionalLight(directionalLights[i], normal, viewDir);
    }

    for(int i = 0; i < numPointLights; ++i)
    {
        result += CalcPointLight(pointLights[i], normal, vPosition, viewDir);
    }

    fragColor = vec4(ambient + result, 1.0);
}
