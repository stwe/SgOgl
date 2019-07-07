#version 330

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D diffuseMap;
uniform vec3 ambientIntensity;

// Main

void main()
{
    vec4 mapColor = texture(diffuseMap, vUv);
    fragColor = vec4(ambientIntensity, 1.0) * mapColor;
}
