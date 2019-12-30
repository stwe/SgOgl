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

// Uniforms

uniform vec3 ambientIntensity;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

uniform vec3 cameraPosition;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// Global

vec3 fragPos;
vec3 normal;
vec3 diffuse;
float specular;
const float shininess = 1.0;

// Function

void GetBufferData()
{
    fragPos = texture(gPosition, vUv).rgb;
    normal = texture(gNormal, vUv).rgb;
    diffuse = texture(gAlbedoSpec, vUv).rgb;
    specular = texture(gAlbedoSpec, vUv).a;
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
    vec3 lightDir = normalize(pointLight.position - fragPos);

    // ambient
    vec3 ambientCol = pointLight.ambientIntensity * diffuse;

    // diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuseCol = pointLight.diffuseIntensity * diffuseFactor * diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularCol = pointLight.specularIntensity * specularFactor * specular;

    // attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

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

    vec3 result = CalcDirectionalLight(normal, viewDir);
    result += CalcPointLight(pointLight, normal, fragPos, viewDir);

    fragColor = vec4(ambient + result, 1.0);
}
