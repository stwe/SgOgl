#version 330

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D diffuseMap;
uniform vec3 ambientIntensity;

// Main

void main()
{
    vec4 mapColor = texture(diffuseMap, vUv);

//    if (mapColor.a < 0.5)
//    {
//        discard;
//    }

    fragColor = vec4(ambientIntensity, 1.0) * mapColor;
}
