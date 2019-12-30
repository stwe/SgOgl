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

// Uniforms

uniform vec3 diffuseColor;
uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;

uniform vec3 specularColor;
uniform float hasSpecularMap;
uniform sampler2D specularMap;

// Global

vec4 diffuseCol;
vec4 specularCol;

// Function

void SetupColors()
{
    // get diffuse color
    diffuseCol = vec4(diffuseColor, 1.0);
    if (hasDiffuseMap > 0.5)
    {
        diffuseCol = texture(diffuseMap, vUv);
    }

    // get specular color
    specularCol = vec4(specularColor, 1.0);
    if (hasSpecularMap > 0.5)
    {
        specularCol = texture(specularMap, vUv);
    }
}

// Main

void main()
{
    gPosition = vPosition;
    gNormal = normalize(vNormal);

    SetupColors();

    gAlbedoSpec.rgb = diffuseCol.rgb;
    gAlbedoSpec.a = specularCol.r;
}
