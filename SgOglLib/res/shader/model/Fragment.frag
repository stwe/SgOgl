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

uniform int numScenePointLights;
uniform int numEntityPointLights;

uniform vec3 ambientIntensity;

uniform float hasDirectionalLight;
uniform DirectionalLight directionalLight;

uniform PointLight scenePointLights[12];  // max 12 point lights
uniform PointLight entityPointLights[12]; // max 12 point lights

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

vec4 diffuse;
vec4 specular;

// Function

vec4 GetDiffuseColor()
{
    vec4 diffuse = vec4(diffuseColor, 1.0);
    if (hasDiffuseMap > 0.5)
    {
        diffuse = texture(diffuseMap, vUv);
    }

    return diffuse;
}

vec4 GetSpecularColor()
{
    vec4 specular = vec4(specularColor, 1.0);
    if (hasSpecularMap > 0.5)
    {
        specular = texture(specularMap, vUv);
    }

    return specular;
}

vec3 GetNormal()
{
    // tangent space
    if (hasNormalMap > 0.5)
    {
        // obtain normal from normal map in range [0, 1]
        vec3 normal = texture(normalMap, vUv).rgb;

        // transform normal vector to range [-1, 1]
        normal = normalize(normal * 2.0 - 1.0);

        return normal;
    }

    // world space
    return normalize(vNormal);
}

vec3 GetViewDir()
{
    // tangent space
    if (hasNormalMap > 0.5)
    {
        vec3 tangentFragmentPosition = vTbnMatrix * vPosition;
        vec3 tangentCameraPosition = vTbnMatrix * cameraPosition;

        return normalize(tangentCameraPosition - tangentFragmentPosition);
    }

    // world space
    return normalize(cameraPosition - vPosition);
}

vec3 GetFragPos()
{
    // tangent space
    if (hasNormalMap > 0.5)
    {
        return vTbnMatrix * vPosition;
    }

    // world space
    return vPosition;
}

vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir)
{
    // negate the global light direction vector to switch its direction
    // it's now a direction vector pointing towards the light source
    vec3 lightDir = normalize(-directionalLight.direction); // in world or tangent space
    if (hasNormalMap > 0.5)
    {
        vec3 tangentDirectionalLightDir = vTbnMatrix * directionalLight.direction;
        lightDir = normalize(-tangentDirectionalLightDir);
    }

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
    // light position in world space or tangent space
    vec3 lightPos = light.position;
    if (hasNormalMap > 0.5)
    {
        lightPos = vTbnMatrix * light.position;
    }

    // light direction
    vec3 lightDir = normalize(lightPos - fragPos);

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
    float distance = length(lightPos - fragPos);
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
    // get colors
    diffuse = GetDiffuseColor();
    specular = GetSpecularColor();

    // discard if transparent
    if (diffuse.a < 0.5)
    {
        discard;
    }

    // get normal in tangent or world space
    vec3 normal = GetNormal();

    // get view direction in tangent or world space
    vec3 viewDir = GetViewDir();

    // calc ambient
    vec3 ambient = ambientIntensity * diffuse.rgb;

    // init result
    vec3 result = vec3(0.0, 0.0, 0.0);

    // calc directional light
    if (hasDirectionalLight > 0.5)
    {
        result = CalcDirectionalLight(normal, viewDir);
    }

    // get fragment position in tangent or world space
    vec3 fragPos = GetFragPos();

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

    // result
    fragColor = vec4(ambient + result, 1.0);
}
