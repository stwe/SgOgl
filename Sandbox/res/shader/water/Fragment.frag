#version 330

// water/Fragment.frag

// In

in vec4 vClipSpace;
in vec2 vUv;
in vec3 vToCameraVector;
in vec3 vFromLightVector;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float moveFactor;
uniform vec3 lightColor;

// Main

const float waveStrength = 0.04;
const float shineDamper = 20.0;
const float reflectivity = 0.5;

void main()
{
    vec2 ndc = (vClipSpace.xy / vClipSpace.w) / 2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    float near = 0.1;
    float far = 10000.0;

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

    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 viewVector = normalize(vToCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 0.5);

    vec3 reflectedLight = reflect(normalize(vFromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth, 0.0, 1.0);

    fragColor = mix(reflectionColor, refractionColor, refractiveFactor);
    fragColor = mix(fragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);
    fragColor.a = clamp(waterDepth / 1.5, 0.0, 1.0);
}
