#version 330

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

// Out

out vec3 gsNormal;

// Uniforms

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Main

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    gsNormal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));
}
