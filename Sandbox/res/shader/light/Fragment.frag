#version 330

// light/Fragment.frag

// In

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUv;

// Out

out vec4 fragColor;

// Types

struct Material
{
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    float hasDiffuseMap;
    float hasSpecularMap;
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

uniform vec3 viewPos;
uniform sampler2D diffuseMap;
uniform Material material;
uniform PointLight pointLight;

// Main

void main()
{
    vec3 diffuseMapColor = texture(diffuseMap, vUv).rgb;

    // ambient
    vec3 ambient = pointLight.ambientIntensity * diffuseMapColor;

    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(pointLight.position - vPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuseIntensity * diff * diffuseMapColor;

    // specular
    vec3 viewDir = normalize(viewPos - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLight.specularIntensity * spec * diffuseMapColor;

    // attenuation
    float distance = length(pointLight.position - vPosition);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // result
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
