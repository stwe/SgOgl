#version 330

// instancing_with_lighting/Fragment.frag

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
    vec4 textureCol = texture(diffuseMap, vUv);

    if (textureCol.a < 0.5)
    {
        discard;
    }

    // ambient
    vec3 ambient = ambientIntensity * textureCol.rgb;

    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(directionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuseIntensity * diff * textureCol.rgb;

    vec3 result = ambient + diffuse;

    fragColor = vec4(result, 1.0);
}
