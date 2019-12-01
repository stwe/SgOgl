#version 400

// gui_depth_map/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D guiTexture;
uniform float near_plane;
uniform float far_plane;

// Main

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
    float depthValue = texture(guiTexture, vUv).r;
    fragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0);
}
