#version 330

// gbuffer/Fragment.frag

// Out

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

// In

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUv;
in vec3 vTangent;

// Uniforms

uniform vec3 diffuseColor;
uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;

uniform vec3 specularColor;
uniform float hasSpecularMap;
uniform sampler2D specularMap;

uniform float hasNormalMap;
uniform sampler2D normalMap;

// Function

vec3 GetDiffuse()
{
    vec4 diffuse = vec4(diffuseColor, 1.0);
    if (hasDiffuseMap > 0.5)
    {
        diffuse = texture(diffuseMap, vUv);
    }

    return diffuse.rgb;
}

float GetSpecularIntensity()
{
    vec4 specular = vec4(specularColor, 1.0);
    if (hasSpecularMap > 0.5)
    {
        specular = texture(specularMap, vUv);
    }

    return specular.r;
}

vec3 GetNormal()
{
    if (hasNormalMap > 0.5)
    {
        vec3 N = normalize(vNormal);
        vec3 T = normalize(vTangent);
        vec3 B = cross(N, T);
        mat3 TBN = mat3(T, B, N);

        vec3 normal = texture(normalMap, vUv).xyz * 2.0 - vec3(1.0);
        return TBN * normalize(normal);
    }

    return normalize(vNormal);
}

// Main

void main()
{
    gPosition = vPosition;
    gNormal = GetNormal();
    gAlbedoSpec.rgb = GetDiffuse();
    gAlbedoSpec.a = GetSpecularIntensity();
}
