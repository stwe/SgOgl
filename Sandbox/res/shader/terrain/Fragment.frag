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

// Uniforms

uniform vec3 ambientIntensity;
uniform DirectionalLight directionalLight;
uniform vec3 cameraPosition;
uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D rock;
uniform sampler2D normalmap;
uniform sampler2D splatmap;

// Main

void main()
{
    vec3 normal = normalize(texture(normalmap, vUvOrig).rbg);
    vec4 blendValues = texture(splatmap, vUvOrig).rgba;

    // color
    vec3 grass = texture(grass, vUv).rgb * blendValues.r;
    vec3 sand = texture(sand, vUv).rgb * blendValues.g;
    vec3 rock = texture(rock, vUv).rgb * blendValues.b;
    vec3 color = grass + sand + rock;

    // ambient
    vec3 ambient = ambientIntensity * color;

    // directionalLight - diffuse
    vec3 lightDir = normalize(-directionalLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuseIntensity * diff * color;

    // directionalLight - specular
    float shininess = 12.0;
    vec3 viewDir = normalize(cameraPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = directionalLight.specularIntensity * spec * vec3(0.1, 0.2, 0.1);

    // result
    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0);
}
