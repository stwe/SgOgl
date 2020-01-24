#version 430

// terrain_quadtree/Fragment.frag

// In


// Out

out vec4 fragColor;

// Uniforms

uniform vec3 color;

// Main

void main()
{
    //fragColor = vec4(color, 1.0);
    fragColor = vec4(0.1, 0.6, 0.1, 1.0);
}
