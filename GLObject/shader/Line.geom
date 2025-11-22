#version 440 core

layout(lines) in;
layout(triangle_strip, max_vertices=4) out;
uniform float lineWidth;

void main() {
    vec2 firstPoint = vec2(gl_in[0].gl_Position.xy);
    vec2 secondPoint = vec2(gl_in[1].gl_Position.xy);
    vec2 direction = normalize(firstPoint - secondPoint);
    vec2 normal = vec2(direction.y, -direction.x);
    vec2 offset = normal * lineWidth * 0.5f;

    vec2 quad[4] = {
        firstPoint - offset,
        firstPoint + offset,
        secondPoint + offset,
        secondPoint - offset
    };

    for( int i = 0; i < 4; i++)
    {
        gl_Position = vec4(quad[i], 0.0f, 1.0f);
        EmitVertex();
    }

    EndPrimitive();
}