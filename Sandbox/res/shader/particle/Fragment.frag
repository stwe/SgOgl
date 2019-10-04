#version 330

// particle/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform vec4 color;
uniform sampler2D particleTexture;

// Main

void main()
{
    vec4 mapColor = texture(particleTexture, vUv);

    if (mapColor.a < 0.5)
    {
        discard;
    }

    fragColor = mapColor;
    //fragColor = color;
}
