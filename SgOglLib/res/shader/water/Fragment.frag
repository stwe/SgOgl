#version 330

// water/Fragment.frag

// In

in vec3 vWorldPosition;
in vec4 vClipSpace;
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

uniform float hasDirectionalLight;
uniform DirectionalLight directionalLight;
uniform vec3 cameraPosition;
uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float moveFactor;
uniform float near;
uniform float far;
uniform float waveStrength;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 waterColor;

// Main

void main()
{
    vec2 ndc = (vClipSpace.xy / vClipSpace.w) / 2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    float depth = texture(depthMap, refractTexCoords).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    float waterDepth = floorDistance - waterDistance;

    vec2 distortedTexCoords = texture(dudvMap, vec2(vUv.x + moveFactor, vUv.y)).rg * 0.1;
    distortedTexCoords = vUv + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth, 0.0, 1.0);

    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    reflectTexCoords += totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

    vec4 reflectionColor = texture(reflectionMap, reflectTexCoords);

    vec4 refractionColor = texture(refractionMap, refractTexCoords);
    refractionColor = mix(refractionColor, vec4(waterColor, 1.0), clamp(waterDepth / 70.0, 0.0, 1.0));

    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);



    float refractiveFactor = 0.5;
    vec3 specularHighlights = vec3(0.5, 0.5, 0.5);

    // calc view direction
    vec3 viewDir = normalize(cameraPosition - vWorldPosition);

    // calc directional light
    if (hasDirectionalLight > 0.5)
    {
        refractiveFactor = dot(viewDir, normal);
        refractiveFactor = pow(refractiveFactor, 0.6);
        refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);

        vec3 reflectedLight = reflect(normalize(-directionalLight.direction), normal);
        float specular = max(dot(reflectedLight, viewDir), 0.0);
        specular = pow(specular, shineDamper);
        specularHighlights = directionalLight.specularIntensity * specular * reflectivity * clamp(waterDepth / 0.5, 0.0, 1.0);
    }



    fragColor = mix(reflectionColor, refractionColor + vec4(specularHighlights, 0.0), refractiveFactor);
    fragColor = mix(fragColor, vec4(waterColor, 1.0), 0.2);
    fragColor.a = clamp(waterDepth / 0.5, 0.0, 1.0);
}
