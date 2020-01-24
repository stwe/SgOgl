#version 430

// terrain_quadtree/Fragment.frag

// In

in vec2 mapCoord_FS;

// Out

out vec4 fragColor;

// Uniforms

uniform vec3 color;
uniform sampler2D normalmap;

// Function

float Diffuse(vec3 t_direction, vec3 t_normal, float t_intensity)
{
    return max(0.01, dot(t_normal, -(t_direction) * t_intensity));
}

// Const

const vec3 lightDirection = vec3(0.1f, -1.0f, 0.1);
const float intensity = 1.2;

// Main

void main()
{
    vec3 normal = texture(normalmap, mapCoord_FS).rgb;

    float diff = Diffuse(lightDirection, normal, intensity);

    //fragColor = vec4(color, 1.0);
    fragColor = vec4(0.1, 0.6, 0.1, 1.0) * diff;
}
