#version 330

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D grassTexture;

// Main

void main()
{
    fragColor = texture(grassTexture, vUv);
}
