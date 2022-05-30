#version 400 core
in vec4 vPosition;

uniform mat4 view;
uniform mat4 projection;

void main()
{
     gl_Position = projection * view * vec4(vPosition.xy, -vPosition.z, 1);
}

