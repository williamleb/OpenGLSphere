#version 400 core

uniform vec3 uColor;

out vec4 fColor;

void
main()
{
    fColor = vec4(uColor, 1);
}
