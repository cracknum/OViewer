#version 440 core

layout(lines) in;
layout(triangle_strip, max_vertices=4) out;
uniform int lineWidth;
uniform vec2 viewPortSize;

void main() {
    // 将顶点转换到屏幕空间
    vec2 firstPoint = (gl_in[0].gl_Position.xy * 0.5 + 0.5) * viewPortSize;
    vec2 secondPoint = (gl_in[1].gl_Position.xy * 0.5 + 0.5) * viewPortSize;

    // 在屏幕空间中计算
    vec2 direction = normalize(secondPoint - firstPoint);
    vec2 normal = vec2(-direction.y, direction.x);

    // 屏幕空间中的偏移（直接使用像素单位）
    vec2 offset = normal * lineWidth * 0.5;

    // 应用偏移
    vec2 quad[4] = vec2[](
    firstPoint - offset,
    firstPoint + offset,
    secondPoint - offset,
    secondPoint + offset
    );

    // 转换回NDC空间
    for (int i = 0; i < 4; i++) {
        vec2 screenPos = quad[i];
        vec2 ndcPos = (screenPos / viewPortSize) * 2.0 - 1.0;
        gl_Position = vec4(ndcPos, 0.0, 1.0);
        EmitVertex();
    }

    EndPrimitive();
}
