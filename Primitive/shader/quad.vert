#version 440 core

layout(location=0) in vec3 position;
// layout(location=1) in vec3 texPos;
uniform mat4 projectMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 vTexPos;
void main()
{
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
    // vTexPos = texPos;
}
