#version 330

// model/Fragment.frag

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

// Uniforms

uniform vec3 ambientIntensity;
uniform DirectionalLight directionalLight;

uniform vec3 cameraPosition;

uniform vec3 diffuseColor;
uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;

uniform vec3 specularColor;
uniform float hasSpecularMap;
uniform sampler2D specularMap;

uniform float shininess;

// Main

void main()
{
    // get diffuse color
    vec4 diffuseColor = vec4(diffuseColor, 1.0);

    if (hasDiffuseMap > 0.5)
    {
        diffuseColor = texture(diffuseMap, vUv);
    }

    if (diffuseColor.a < 0.5)
    {
        discard;
    }

    // get specular color
    vec4 specularColor = vec4(specularColor, 1.0);

    if (hasSpecularMap > 0.5)
    {
        specularColor = texture(specularMap, vUv);
    }

    // ambient
    vec3 ambient = ambientIntensity * diffuseColor.rgb;

    // directionalLight - diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(-directionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuseIntensity * diff * diffuseColor.rgb;

    // directionalLight - specular
    vec3 viewDir = normalize(cameraPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = directionalLight.specularIntensity * spec * specularColor.rgb;

    // result
    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0);
}
