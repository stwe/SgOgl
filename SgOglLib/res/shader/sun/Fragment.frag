#version 400

// sun/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D sunTexture;

// Main

void main()
{
    vec4 diffuse = texture(sunTexture, vUv);
    fragColor = diffuse;
}
