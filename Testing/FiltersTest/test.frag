#version 440 core

out vec4 fragColor;
in vec4 vertexColor;
in vec2 texturePos;

uniform sampler2D tex1;

void main()
{
  float gray = texture(tex1, texturePos).x;
  fragColor = vec4(gray, gray, gray, 1.0f);
}