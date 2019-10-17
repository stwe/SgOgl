#version 400

// gui/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D guiTexture;

// Main

void main()
{
    fragColor = texture(guiTexture, vUv);
}
