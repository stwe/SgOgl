#version 330

// model/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform vec3 diffuseColor;
uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;
uniform vec3 ambientIntensity;

// Main

void main()
{
    vec4 mapColor = vec4(diffuseColor, 1.0);

    if (hasDiffuseMap > 0.5)
    {
        mapColor = texture(diffuseMap, vUv);
    }

    if (mapColor.a < 0.5)
    {
        discard;
    }

    fragColor = vec4(ambientIntensity, 1.0) * mapColor;
}
