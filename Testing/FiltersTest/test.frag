#version 440 core

out vec4 fragColor;
in vec4 vertexColor;
in vec2 texturePos;
uniform vec2 windowLevel;

uniform sampler2D tex1;

void main()
{
    float gray = texture(tex1, texturePos).x;
    float window = windowLevel.x;
    float level = windowLevel.y;

    float tValue = (gray - (level - window * 0.5)) / window;

    float cValue = clamp(tValue, 0.0f, 1.0f);
    fragColor = vec4(cValue, cValue, cValue, 1.0f);
}