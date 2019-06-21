#version 330

// In

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

// Out

out vec3 ourColor;

// Uniforms

//uniform mat4 mvpMatrix;

// Main

void main()
{
//  gl_Position = mvpMatrix * vec4(aPosition, 1.0);
    gl_Position = vec4(aPosition, 1.0);

    ourColor = aColor;
}
