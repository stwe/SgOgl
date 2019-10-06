#version 330

// particle_anim/Fragment.frag

// In

in vec2 vUv1;
in vec2 vUv2;
in float vBlend;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D particleTexture;

// Main

void main()
{
    vec4 mapColor1 = texture(particleTexture, vUv1);
    vec4 mapColor2 = texture(particleTexture, vUv2);

    fragColor = mix(mapColor1, mapColor2, vBlend);
}
