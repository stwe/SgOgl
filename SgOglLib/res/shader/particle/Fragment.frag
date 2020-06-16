#version 330

// particle/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D particleTexture;

// Main

void main()
{
    fragColor = texture(particleTexture, vUv);
}
