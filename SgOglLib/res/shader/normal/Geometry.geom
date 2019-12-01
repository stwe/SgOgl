#version 330

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

// In

in vec3 vNormal[];

// Out

out vec3 gNormal;

// Uniforms

uniform mat4 transform;
uniform float normalLength;

// Main

void main()
{
   for (int i = 0; i < 3; i++)
   {
       gl_Position = gl_in[i].gl_Position;
       gNormal = vNormal[i];
       EmitVertex();

       gl_Position = gl_in[i].gl_Position + transform * vec4(vNormal[i] * normalLength, 0.0);
       gNormal = vNormal[i];
       EmitVertex();

       EndPrimitive();
   }
}
