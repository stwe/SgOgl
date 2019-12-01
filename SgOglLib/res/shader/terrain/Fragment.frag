#version 330

// terrain/Fragment.frag

// In

in vec3 vPosition;
in vec2 vUvOrig;
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

uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D rock;
uniform sampler2D normalmap;
uniform sampler2D splatmap;

// Global

vec3 diffuseCol;
vec3 specularCol;
const float shininess = 12.0;

// Function

void SetupColors()
{
    vec4 blendValues = texture(splatmap, vUvOrig).rgba;

    vec3 grass = texture(grass, vUv).rgb * blendValues.r;
    vec3 sand = texture(sand, vUv).rgb * blendValues.g;
    vec3 rock = texture(rock, vUv).rgb * blendValues.b;

    diffuseCol = grass + sand + rock;
    specularCol = vec3(0.1, 0.2, 0.1);
}

vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-directionalLight.direction);

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
    vec3 lightDir = normalize(pointLight.position - fragPos);

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

    vec3 normal = normalize(texture(normalmap, vUvOrig).rbg);
    vec3 viewDir = normalize(cameraPosition - vPosition);

    vec3 ambient = ambientIntensity * diffuseCol;

    vec3 result = CalcDirectionalLight(normal, viewDir);
    result += CalcPointLight(pointLight, normal, vPosition, viewDir);

    fragColor = vec4(ambient + result, 1.0);
}
