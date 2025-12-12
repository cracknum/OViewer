#version 440 core

out vec4 fragColor;
in vec4 vertexColor;
void main()
{
    fragColor = vec4(
        clamp((vertexColor[0] + 1.0f), 0.0f, 1.0f),
        clamp((vertexColor[1] + 1.0f), 0.0f, 1.0f),
        clamp((vertexColor[2] + 1.0f), 0.0f, 1.0f),
         1.0f
    );
}