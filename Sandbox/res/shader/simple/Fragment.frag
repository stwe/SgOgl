#version 330

// In

in vec3 ourColor;
in vec2 texCoord;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D ourTexture;

// Main

void main()
{
    //fragColor = vec4(ourColor, 1.0);
    fragColor = texture(ourTexture, texCoord);
}
