#version 440 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 texPos;

out vec3 vTexPos;
void main()
{
    gl_Position = vec4(position, 1.0f);
    vTexPos = texPos;
}
