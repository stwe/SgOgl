#version 330

// In

in vec3 ourColor;

// Out

out vec4 fragColor;

// Uniforms

// Main

void main()
{
    fragColor = vec4(ourColor, 1.0);
}
