#version 330

// skybox/Fragment.frag

// In

in vec3 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform samplerCube cubeSampler;

// Main

void main()
{
    fragColor = texture(cubeSampler, vUv);
}
