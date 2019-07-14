#version 330

// In

in vec3 gNormal;

// Out

out vec4 fragColor;

// Main

void main()
{
    fragColor = vec4(normalize(gNormal) * 0.5 + 0.5, 1.0);
}
