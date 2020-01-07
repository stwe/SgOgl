#version 330

// lighting_pass/Fragment.frag

// In

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

// Const

const float shininess = 24.0;

// Uniforms

uniform int numScenePointLights;
uniform int numEntityPointLights;

uniform vec3 ambientIntensity;

uniform float hasDirectionalLight;
uniform DirectionalLight directionalLight;

uniform PointLight scenePointLights[12];  // max 12 point lights
uniform PointLight entityPointLights[12]; // max 12 point lights

uniform vec3 cameraPosition;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// Global

vec3 fragPos;
vec3 normal;
vec3 diffuse;
float specular;

// Function

void GetBufferData()
{
    fragPos = texture(gPosition, vUv).rgb;
    normal = texture(gNormal, vUv).rgb;
    diffuse = texture(gAlbedoSpec, vUv).rgb;
    specular = texture(gAlbedoSpec, vUv).a;

    if (normal == vec3(0.0, 0.0, 0.0))
    {
        discard;
    }
}

vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-directionalLight.direction);

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuseCol = directionalLight.diffuseIntensity * diffuseFactor * diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularCol = directionalLight.specularIntensity * specularFactor * specular;

    // result
    return diffuseCol + specularCol;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // ambient
    vec3 ambientCol = light.ambientIntensity * diffuse;

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuseCol = light.diffuseIntensity * diffuseFactor * diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularCol = light.specularIntensity * specularFactor * specular;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambientCol *= attenuation;
    diffuseCol *= attenuation;
    specularCol *= attenuation;

    // result
    return ambientCol + diffuseCol + specularCol;
}

// Main

void main()
{
    GetBufferData();

    vec3 ambient = ambientIntensity * diffuse;
    vec3 viewDir = normalize(cameraPosition - fragPos);

    // init result
    vec3 result = vec3(0.0, 0.0, 0.0);

    // calc directional light
    if (hasDirectionalLight > 0.5)
    {
        result = CalcDirectionalLight(normal, viewDir);
    }

    // calc scene point lights
    for(int i = 0; i < numScenePointLights; ++i)
    {
        result += CalcPointLight(scenePointLights[i], normal, fragPos, viewDir);
    }

    // calc entity point lights
    for(int i = 0; i < numEntityPointLights; ++i)
    {
        result += CalcPointLight(entityPointLights[i], normal, fragPos, viewDir);
    }

    fragColor = vec4(ambient + result, 1.0);
}
