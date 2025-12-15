#version 440 core

layout(location=0) in vec2 inPos;
layout(location=1) in vec2 inTexturePos;

out vec4 vertexColor;
out vec2 texturePos;
void main()
{
    gl_Position = vec4(inPos, 0.0f, 1.0f);
    vertexColor = gl_Position;
    texturePos = inTexturePos;
}