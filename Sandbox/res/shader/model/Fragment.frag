#version 330

// In

in vec2 texCoord;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D ourTexture;

// Main

void main()
{
    fragColor = texture(ourTexture, texCoord);
}
