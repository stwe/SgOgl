#version 330

// terrain/Fragment.frag

// In

in vec2 vUvOrig;
in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D rock;
uniform sampler2D normalmap;
uniform sampler2D splatmap;

// Main

void main()
{
    vec3 normal = normalize(texture(normalmap, vUvOrig).rbg);
    vec4 blendValues = texture(splatmap, vUvOrig).rgba;

    vec3 grass = texture(grass, vUv).rgb * blendValues.r;
    vec3 sand = texture(sand, vUv).rgb * blendValues.g;
    vec3 rock = texture(rock, vUv).rgb * blendValues.b;

    vec3 res = grass + sand + rock;

    fragColor = vec4(res, 1.0);
//    fragColor = vec4(normal, 1.0);
//    fragColor = blendValues;
}
