#version 330

// dome/Fragment.frag

// In

in vec3 worldPosition;

// Out

out vec4 fragColor;

// Main

const vec3 baseColor = vec3(0.18, 0.27, 0.47);

void main()
{
    float red = -0.00022 * (abs(worldPosition.y)-2800) + 0.18;
    float green = -0.00025 * (abs(worldPosition.y)-2800) + 0.27;
    float blue = -0.00019 * (abs(worldPosition.y)-2800) + 0.47;

    fragColor = vec4(red, green, blue, 1.0);
}
