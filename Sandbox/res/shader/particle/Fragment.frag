#version 330

// particle/Fragment.frag

// In

in vec2 vUv;
in vec4 vColor;

// Out

out vec4 fragColor;

// Uniforms

// Main

void main()
{
    fragColor = vColor;
}
