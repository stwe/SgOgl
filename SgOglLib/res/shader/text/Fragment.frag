#version 330

// text/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D textTexture;
uniform vec3 textColor;

// Main

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, vUv).r);
    fragColor = vec4(textColor, 1.0) * sampled;
}
