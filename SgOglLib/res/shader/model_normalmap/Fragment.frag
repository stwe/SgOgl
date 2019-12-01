#version 330

// model_normalmap/Fragment.frag

// In

in vec2 vUv;

in vec3 vTangentFragmentPosition;
in vec3 vTangentDirectionalLightDirection;
in vec3 vTangentPointLightPosition;
in vec3 vTangentCameraPosition;

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

uniform vec3 ambientIntensity;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

uniform vec3 diffuseColor;
uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;

uniform vec3 specularColor;
uniform float hasSpecularMap;
uniform sampler2D specularMap;

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

vec3 GetNormal()
{
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, vUv).rgb;

    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    return normal;
}

vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-vTangentDirectionalLightDirection);

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
    vec3 lightDir = normalize(vTangentPointLightPosition - fragPos);

    // ambient
    vec3 ambient = pointLight.ambientIntensity * diffuseCol.rgb;

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuseIntensity * diffuseFactor * diffuseCol.rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = pointLight.specularIntensity * specularFactor * specularCol.rgb;

    // attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

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

    if (diffuseCol.a < 0.5)
    {
        discard;
    }

    vec3 normal = GetNormal();
    vec3 viewDir = normalize(vTangentCameraPosition - vTangentFragmentPosition);

    vec3 ambient = ambientIntensity * diffuseCol.rgb;

    vec3 result = CalcDirectionalLight(normal, viewDir);
    result += CalcPointLight(pointLight, normal, vTangentFragmentPosition, viewDir);

    fragColor = vec4(ambient + result, 1.0);
    //fragColor = vec4(normal, 1.0);
}
