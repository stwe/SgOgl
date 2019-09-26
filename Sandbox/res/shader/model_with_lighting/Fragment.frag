#version 330

// model_with_lighting/Fragment.frag

// In

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
uniform sampler2D diffuseMap;

// Main

void main()
{
    vec3 textureCol = texture(diffuseMap, vUv).rgb;

    // ambient
    vec3 ambient = ambientIntensity * textureCol;

    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(directionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuseIntensity * diff * textureCol;

    // specular
//    float shininess = 32.0;
//    vec3 viewDir = normalize(cameraPosition - vPosition);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
//    vec3 specular = directionalLight.specularIntensity * spec * textureCol;

//    vec3 result = ambient + diffuse + specular;
    vec3 result = ambient + diffuse;

    fragColor = vec4(result, 1.0);
}
