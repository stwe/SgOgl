#version 330

// water/Fragment.frag

// In

in vec4 vClipSpace;
in vec2 vUv;
in vec3 vToCameraVector;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float moveFactor;

// Main

const float waveStrength = 0.02;

void main()
{
    vec2 ndc = (vClipSpace.xy / vClipSpace.w) / 2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    //vec2 distortion1 = (texture(dudvMap, vec2(vUv.x + moveFactor, vUv.y)).rg * 2.0 - 1.0) * waveStrength;
    //vec2 distortion2 = (texture(dudvMap, vec2(-vUv.x + moveFactor, vUv.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
    //vec2 totalDistortion = distortion1 + distortion2;

    vec2 distortedTexCoords = texture(dudvMap, vec2(vUv.x + moveFactor, vUv.y)).rg * 0.1;
    distortedTexCoords = vUv + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    reflectTexCoords += totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

    vec4 reflectionColor = texture(reflectionMap, reflectTexCoords);
    vec4 refractionColor = texture(refractionMap, refractTexCoords);

    vec3 viewVector = normalize(vToCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
    refractiveFactor = pow(refractiveFactor, 0.5);

    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);


    // todo light position


    fragColor = mix(reflectionColor, refractionColor, refractiveFactor);
    fragColor = mix(fragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}
