#version 330

// model/Fragment.frag

// In

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUv;
in mat3 vTbnMatrix;

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

uniform int numLights;

uniform vec3 ambientIntensity;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[12]; // max 12 point lights

uniform vec3 cameraPosition;

uniform vec3 diffuseColor;
uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;

uniform vec3 specularColor;
uniform float hasSpecularMap;
uniform sampler2D specularMap;

uniform float hasNormalMap;
uniform sampler2D normalMap;

uniform float shininess;

// Global

vec4 diffuseCol;
vec4 specularCol;

// Function

void SetupColors()
{
    // get diffuse color
    diffuseCol = vec4(diffuseColor, 1.0);
    if (hasDiffuseMap > 0.5)
    {
        diffuseCol = texture(diffuseMap, vUv);
    }

    // get specular color
    specularCol = vec4(specularColor, 1.0);
    if (hasSpecularMap > 0.5)
    {
        specularCol = texture(specularMap, vUv);
    }
}

vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-directionalLight.direction);
    if (hasNormalMap > 0.5)
    {
        vec3 tangentDirectionalLightDirection = vTbnMatrix * directionalLight.direction;
        lightDir = normalize(-tangentDirectionalLightDirection);
    }

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuseIntensity * diffuseFactor * diffuseCol.rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = directionalLight.specularIntensity * specularFactor * specularCol.rgb;

    // result
    return diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    if (hasNormalMap > 0.5)
    {
        vec3 tangentPointLightPosition = vTbnMatrix * light.position;
        lightDir = normalize(tangentPointLightPosition - fragPos);
    }

    // ambient
    vec3 ambient = light.ambientIntensity * diffuseCol.rgb;

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuseIntensity * diffuseFactor * diffuseCol.rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specularIntensity * specularFactor * specularCol.rgb;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // result
    return ambient + diffuse + specular;
}

vec3 GetNormal()
{
    if (hasNormalMap > 0.5)
    {
        // obtain normal from normal map in range [0,1]
        vec3 normal = texture(normalMap, vUv).rgb;

        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

        return normal;
    }

    return normalize(vNormal);
}

vec3 GetViewDir()
{
    if (hasNormalMap > 0.5)
    {
        vec3 tangentFragmentPosition = vTbnMatrix * vPosition;
        vec3 tangentCameraPosition = vTbnMatrix * cameraPosition;

        return normalize(tangentCameraPosition - tangentFragmentPosition);
    }

    return normalize(cameraPosition - vPosition);
}

// Main

void main()
{
    SetupColors();

    if (diffuseCol.a < 0.5)
    {
        discard;
    }

    vec3 normal = GetNormal();
    vec3 viewDir = GetViewDir();

    vec3 ambient = ambientIntensity * diffuseCol.rgb;

    vec3 result = CalcDirectionalLight(normal, viewDir);

    for(int i = 0; i < numLights; ++i)
    {
        result += CalcPointLight(pointLights[i], normal, vPosition, viewDir);
    }

    fragColor = vec4(ambient + result, 1.0);
}
