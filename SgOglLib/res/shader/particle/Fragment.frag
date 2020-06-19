#version 330

// particle/Fragment.frag

// In

in vec2 vUvCurrent;
in vec2 vUvNext;
in float vBlend;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D particleTexture;

// Main

void main()
{
    vec4 col0 = texture(particleTexture, vUvCurrent);
    vec4 col1 = texture(particleTexture, vUvNext);

    fragColor = mix(col0, col1, vBlend);
}
