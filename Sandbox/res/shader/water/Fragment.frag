#version 330

// water/Fragment.frag

// In

in vec4 clipSpace;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;

// Main

void main()
{
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    vec4 reflectionColor = texture(reflectionMap, reflectTexCoords);
    vec4 refractionColor = texture(refractionMap, refractTexCoords);

    fragColor = mix(reflectionColor, refractionColor, 0.5);
}
