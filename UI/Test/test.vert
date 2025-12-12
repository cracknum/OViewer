#version 440 core

layout(location=0) in vec2 inPos;

out vec4 vertexColor;
void main()
{
    gl_Position = vec4(inPos, 0.0f, 1.0f);
    vertexColor = gl_Position;
}