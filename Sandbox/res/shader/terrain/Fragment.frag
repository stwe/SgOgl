#version 330

// In

in vec2 vUvOrig;
in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D stone1;
uniform sampler2D stone2;
uniform sampler2D splatmap;

// Main

void main()
{
    vec4 blendValues = texture(splatmap, vUvOrig).rgba;

    vec3 grass = texture(grass, vUv).rgb * blendValues.r;
    vec3 sand = texture(sand, vUv).rgb * blendValues.g;
    vec3 stone1 = texture(stone1, vUv).rgb * blendValues.b;

    vec3 res = grass + sand + stone1;

    fragColor = vec4(res, 1.0);
}
