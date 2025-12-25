// =============================================================================
// csg.glsl —— Constructive Solid Geometry (CSG) Utilities for SDF Rendering
// Author: Based on Inigo Quilez & Community Best Practices
// =============================================================================

#ifndef CSG_GLSL
#define CSG_GLSL

// ----------------------------------------------------------------------------
// 🔷 基础 CSG 布尔操作（精确）
// ----------------------------------------------------------------------------

/**
 * 并集：A ∪ B
 */
float opUnion(float d1, float d2) {
    return min(d1, d2);
}

/**
 * 交集：A ∩ B
 */
float opIntersect(float d1, float d2) {
    return max(d1, d2);
}

/**
 * 差集：A − B
 */
float opSubtract(float d1, float d2) {
    return max(d1, -d2);
}

// ----------------------------------------------------------------------------
// 🔷 平滑 CSG 操作（视觉融合，非精确距离）
// ----------------------------------------------------------------------------

/**
 * 平滑最小值（用于平滑并集）
 * k: 混合半径（>0），越大越平滑
 */
float smin(float a, float b, float k) {
    float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    return mix(b, a, h) - k * h * (1.0 - h);
}

/**
 * 平滑最大值（用于平滑交集/差集）
 */
float smax(float a, float b, float k) {
    return -smin(-a, -b, k);
}

/**
 * 平滑并集
 */
float opSmoothUnion(float d1, float d2, float k) {
    return smin(d1, d2, k);
}

/**
 * 平滑交集
 */
float opSmoothIntersect(float d1, float d2, float k) {
    return smax(d1, d2, k);
}

/**
 * 平滑差集
 */
float opSmoothSubtract(float d1, float d2, float k) {
    return smax(d1, -d2, k);
}

// ----------------------------------------------------------------------------
// 🔷 空间变换与重复
// ----------------------------------------------------------------------------

/**
 * 三维取模重复（立方体阵列）
 * c: 重复周期（如 vec3(3.0) 表示每 3 单位重复一次）
 */
vec3 opRepeat(vec3 p, vec3 c) {
    return mod(p, c) - 0.5 * c;
}

/**
 * 二维取模重复（用于 XY 平面）
 */
vec2 opRepeat2D(vec2 p, vec2 c) {
    return mod(p, c) - 0.5 * c;
}

/**
 * 径向重复（绕 Y 轴环形复制）
 * n: 复制份数（必须为整数）
 */
vec2 opRadialRepeat(vec2 p, float n) {
    const float PI = 3.14159265359;
    float ang = 2.0 * PI / n;
    float sector = floor(atan(p.y, p.x) / ang + 0.5);
    float cos_a = cos(sector * ang);
    float sin_a = sin(sector * ang);
    return vec2(
        p.x * cos_a + p.y * sin_a,
        -p.x * sin_a + p.y * cos_a
    );
}

/**
 * 对称（镜像到第一卦限）
 */
vec3 opSymmetry(vec3 p) {
    return abs(p);
}

// ----------------------------------------------------------------------------
// 🔷 实用宏（提升可读性）
// ----------------------------------------------------------------------------

#define UNION(a, b)             opUnion(a, b)
#define INTERSECT(a, b)         opIntersect(a, b)
#define SUBTRACT(a, b)          opSubtract(a, b)

#define SMOOTH_UNION(a, b, k)   opSmoothUnion(a, b, k)
#define SMOOTH_INTERSECT(a, b, k) opSmoothIntersect(a, b, k)
#define SMOOTH_SUBTRACT(a, b, k) opSmoothSubtract(a, b, k)

#define REPEAT(p, c)            opRepeat(p, c)
#define RADIAL_REPEAT(p, n)     opRadialRepeat(p, n)
#define SYMMETRY(p)             opSymmetry(p)

// ----------------------------------------------------------------------------
// 🔷 辅助函数（若未定义）
// ----------------------------------------------------------------------------

#ifndef PI
#define PI 3.14159265359
#endif

// 2D 旋转（绕原点）
vec2 rotate2D(vec2 v, float angle) {
    float c = cos(angle), s = sin(angle);
    return vec2(v.x * c - v.y * s, v.x * s + v.y * c);
}

// 3D 绕 Y 轴旋转
vec3 rotateY(vec3 v, float angle) {
    return vec3(
        v.x * cos(angle) + v.z * sin(angle),
        v.y,
        -v.x * sin(angle) + v.z * cos(angle)
    );
}

// 点到线段距离（用于胶囊体等）
float distPointToSegment(vec3 p, vec3 a, vec3 b) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h);
}

// 安全 clamp（避免 NaN）
float safeClamp(float x, float a, float b) {
    return clamp(x, min(a, b), max(a, b));
}

#endif // CSG_GLSL