#version 420 core

layout (location = 0) in vec3 aPos;

uniform vec3 color;
uniform mat4 view_projection;
uniform mat4 model;

layout(location = 0) out vec3 vColor;

void main()
{
    gl_Position = view_projection * model * vec4(aPos, 1.0);
    vColor = color;
}
