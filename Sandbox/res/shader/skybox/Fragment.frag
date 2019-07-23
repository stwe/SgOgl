#version 330

// In

in vec3 vUv;

// Out

out vec4 fragColour;

// Uniforms

uniform samplerCube cubeSampler;

// Main

void main()
{
    fragColour = texture(cubeSampler, vUv);
}
