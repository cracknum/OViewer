#ifndef SHAPE_SDF_GLSL
#define SHAPE_SDF_GLSL
// 所有的sdf函数的中心或者原点都在(0,0,0)

float dot2(in vec2 v)
{
  return dot(v, v);
}
float dot2(in vec3 v)
{
  return dot(v, v);
}
float ndot(in vec2 a, in vec2 b)
{
  return a.x * b.x - a.y * b.y;
}

/**
 * 原点定义在原点，法向量为(0,1,0)的平面
 * p: 待测试点
 *
 */
float sdPlane(vec3 p)
{
  return p.y;
}

/**
 * 球心定义在原点的球
 * p: 待测试点
 * s: 半径
 */
float sdSphere(vec3 p, float s)
{
  return length(p) - s;
}

/**
 * 计算一个点 p 到一个轴对齐的长方体（AABB，Axis-Aligned Bounding Box）的距离
 * p: 待测试点
 * b: 长宽高的半边长度(width/2, height/2, depth/2)
 */
float sdBox(vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

/**
  * 用于计算点 p 到一个空心长方体框架（box frame / wireframe box） 的距离。这种形状常用于可视化边界、建筑结构、网格线等
  * 中心点在原点
  * p: 待测试点
  * b: 长方体的半边长度(width/2, height/2, depth/2)
  * e: 边框厚度的一半（half-thickness of the frame）控制“线”的粗细；值越大，边框越粗
*/
float sdBoxFrame(vec3 p, vec3 b, float e)
{
  p = abs(p) - b;
  vec3 q = abs(p + e) - e;

  return min(min(length(max(vec3(p.x, q.y, q.z), 0.0)) + min(max(p.x, max(q.y, q.z)), 0.0),
               length(max(vec3(q.x, p.y, q.z), 0.0)) + min(max(q.x, max(p.y, q.z)), 0.0)),
    length(max(vec3(q.x, q.y, p.z), 0.0)) + min(max(q.x, max(q.y, p.z)), 0.0));
}

/**
  * 用于计算点 p 到一个以原点为中心、主轴与坐标轴对齐的椭球体（ellipsoid） 的距离(非精确解)
  * p: 待测试的 3D 点
  * r: 椭球在 x、y、z 三个主轴方向上的半轴长度（radii）即：椭球方程为 (x/r.x)² + (y/r.y)² + (z/r.z)² = 1
*/
float sdEllipsoid(in vec3 p, in vec3 r) // approximated
{
  float k0 = length(p / r);
  float k1 = length(p / (r * r));
  return k0 * (k0 - 1.0) / k1;
}

/**
  * 用于计算点 p 到一个标准环面（torus） 的距离，中心为原点
  * p: 待测试的3D坐标
  * t: 环面的两个半径，
  *   - t.x = 主半径（major radius）R：从环中心到管中心的距离
  *   - t.y = 管半径（minor radius）r：环管本身的截面半径
*/
float sdTorus(vec3 p, vec2 t)
{
  return length(vec2(length(p.xz) - t.x, p.y)) - t.y;
}

/**
  * 用于表示一种被“截断”或“限幅”的环面（capped torus） —— 更准确地说，它描述的是一个沿 Y 轴方向被两个平面切割后的环面片段
  * 中心在原点
  * p: 待测试点
  * sc: 截断方向的单位向量
  * ra: 主半径（major radius）R，从环中心到管中心的距离
  * rb: 管半径（minor radius）r, 环管截面的半径
*/
float sdCappedTorus(in vec3 p, in vec2 sc, in float ra, in float rb)
{
  p.x = abs(p.x);
  float k = (sc.y * p.x > sc.x * p.y) ? dot(p.xy, sc) : length(p.xy);
  return sqrt(dot(p, p) + ra * ra - 2.0 * ra * k) - rb;
}

/**
  * 用于计算点 p 到一个正六边形柱体（regular hexagonal prism） 的距离
  * p: 待测试点
  * h: 
  *   - h.x = 六边形外接圆半径
  *   - h.y = 柱体半高（half-height）
*/
float sdHexPrism(vec3 p, vec2 h)
{
  vec3 q = abs(p);

  const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
  p = abs(p);
  p.xy -= 2.0 * min(dot(k.xy, p.xy), 0.0) * k.xy;
  vec2 d =
    vec2(length(p.xy - vec2(clamp(p.x, -k.z * h.x, k.z * h.x), h.x)) * sign(p.y - h.x), p.z - h.y);
  return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

/**
 * 正八边形柱体（regular octagonal prism）
 * 底面为正八边形（顶点朝上），沿 Z 轴拉伸
 * 中心在原点，对称于坐标平面
 * p: 待测试点
 * r: 八边形外接圆半径（顶点到中心的距离）
 * h: 柱体半高（总高度为 2*h）
 */
float sdOctogonPrism(in vec3 p, in float r, float h)
{
  const vec3 k = vec3(-0.9238795325, // sqrt(2+sqrt(2))/2
    0.3826834323,                    // sqrt(2-sqrt(2))/2
    0.4142135623);                   // sqrt(2)-1
  // reflections
  p = abs(p);
  p.xy -= 2.0 * min(dot(vec2(k.x, k.y), p.xy), 0.0) * vec2(k.x, k.y);
  p.xy -= 2.0 * min(dot(vec2(-k.x, k.y), p.xy), 0.0) * vec2(-k.x, k.y);
  // polygon side
  p.xy -= vec2(clamp(p.x, -k.z * r, k.z * r), r);
  vec2 d = vec2(length(p.xy) * sign(p.y), p.z - h);
  return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

/**
 * 胶囊体（capsule）：由线段 [a, b] 沿其方向拉伸一个球体形成
 * 可视为“带圆头的圆柱”
 * p: 待测试点
 * a: 线段起点
 * b: 线段终点
 * r: 胶囊半径（球体半径）
 */
float sdCapsule(vec3 p, vec3 a, vec3 b, float r)
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  return length(pa - ba * h) - r;
}

/**
 * 圆锥台（截头圆锥），轴线沿 Y 轴，底面在 y=0，顶面在 y=h
 * p: 待测试点
 * r1: 底面半径（y=0 处）
 * r2: 顶面半径（y=h 处）
 * h: 圆锥高度（>0）
 */
float sdRoundCone(in vec3 p, in float r1, float r2, float h)
{
  vec2 q = vec2(length(p.xz), p.y);

  float b = (r1 - r2) / h;
  float a = sqrt(1.0 - b * b);
  float k = dot(q, vec2(-b, a));

  if (k < 0.0)
    return length(q) - r1;
  if (k > a * h)
    return length(q - vec2(0.0, h)) - r2;

  return dot(q, vec2(a, b)) - r1;
}

/**
 * 任意朝向的圆锥台（截头圆锥）
 * p: 待测试点
 * a: 圆锥底面中心
 * b: 圆锥顶面中心
 * r1: 底面半径（在 a 点处）
 * r2: 顶面半径（在 b 点处）
 */
float sdRoundCone(vec3 p, vec3 a, vec3 b, float r1, float r2)
{
  // sampling independent computations (only depend on shape)
  vec3 ba = b - a;
  float l2 = dot(ba, ba);
  float rr = r1 - r2;
  float a2 = l2 - rr * rr;
  float il2 = 1.0 / l2;

  // sampling dependant computations
  vec3 pa = p - a;
  float y = dot(pa, ba);
  float z = y - l2;
  float x2 = dot2(pa * l2 - ba * y);
  float y2 = y * y * l2;
  float z2 = z * z * l2;

  // single square root!
  float k = sign(rr) * rr * rr * x2;
  if (sign(z) * a2 * z2 > k)
    return sqrt(x2 + z2) * il2 - r2;
  if (sign(y) * a2 * y2 < k)
    return sqrt(x2 + y2) * il2 - r1;
  return (sqrt(x2 * a2 * il2) + y * rr) * il2 - r1;
}

/**
 * 正三角柱体（equilateral triangular prism），底面为等边三角形
 * 三角形一个顶点朝上，沿 Z 轴拉伸
 * 中心在原点
 * p: 待测试点
 * h: vec2(h.x, h.y)，其中 h.x 为三角形高，h.y 为柱体半高
 */
float sdTriPrism(vec3 p, vec2 h)
{
  const float k = sqrt(3.0);
  h.x *= 0.5 * k;
  p.xy /= h.x;
  p.x = abs(p.x) - 1.0;
  p.y = p.y + 1.0 / k;
  if (p.x + k * p.y > 0.0)
    p.xy = vec2(p.x - k * p.y, -k * p.x - p.y) / 2.0;
  p.x -= clamp(p.x, -2.0, 0.0);
  float d1 = length(p.xy) * sign(-p.y) * h.x;
  float d2 = abs(p.z) - h.y;
  return length(max(vec2(d1, d2), 0.0)) + min(max(d1, d2), 0.);
}

/**
 * 圆柱体（轴线沿 Y 轴）
 * p: 待测试点
 * h: vec2(h.x, h.y)，其中 h.x 为底面半径，h.y 为半高
 */
// vertical
float sdCylinder(vec3 p, vec2 h)
{
  vec2 d = abs(vec2(length(p.xz), p.y)) - h;
  return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

/**
 * 任意朝向的圆柱体（由两点 a, b 定义轴线）
 * p: 待测试点
 * a: 圆柱一端中心
 * b: 圆柱另一端中心
 * r: 圆柱半径
 */
// arbitrary orientation
float sdCylinder(vec3 p, vec3 a, vec3 b, float r)
{
  vec3 pa = p - a;
  vec3 ba = b - a;
  float baba = dot(ba, ba);
  float paba = dot(pa, ba);

  float x = length(pa * baba - ba * paba) - r * baba;
  float y = abs(paba - baba * 0.5) - baba * 0.5;
  float x2 = x * x;
  float y2 = y * y * baba;
  float d = (max(x, y) < 0.0) ? -min(x2, y2) : (((x > 0.0) ? x2 : 0.0) + ((y > 0.0) ? y2 : 0.0));
  return sign(d) * sqrt(abs(d)) / baba;
}

/**
 * 圆锥（轴线沿 Y 轴），底面在 y=0，顶点在 (0,h,0)
 * p: 待测试点
 * c: vec2(c.x, c.y) = (底面半径, 高度)
 * h: 实际未使用（函数签名冗余，应为 c.y）
 */
// vertical
float sdCone(in vec3 p, in vec2 c, float h)
{
  vec2 q = h * vec2(c.x, -c.y) / c.y;
  vec2 w = vec2(length(p.xz), p.y);

  vec2 a = w - q * clamp(dot(w, q) / dot(q, q), 0.0, 1.0);
  vec2 b = w - q * vec2(clamp(w.x / q.x, 0.0, 1.0), 1.0);
  float k = sign(q.y);
  float d = min(dot(a, a), dot(b, b));
  float s = max(k * (w.x * q.y - w.y * q.x), k * (w.y - q.y));
  return sqrt(d) * sign(s);
}

/**
 * 带端盖的圆锥台（capped truncated cone），轴线沿 Y 轴
 * p: 待测试点
 * h: 圆锥高度（从 y=-h 到 y=+h）
 * r1: 底面半径（y=-h 处）
 * r2: 顶面半径（y=+h 处）
 */
float sdCappedCone(in vec3 p, in float h, in float r1, in float r2)
{
  vec2 q = vec2(length(p.xz), p.y);

  vec2 k1 = vec2(r2, h);
  vec2 k2 = vec2(r2 - r1, 2.0 * h);
  vec2 ca = vec2(q.x - min(q.x, (q.y < 0.0) ? r1 : r2), abs(q.y) - h);
  vec2 cb = q - k1 + k2 * clamp(dot(k1 - q, k2) / dot2(k2), 0.0, 1.0);
  float s = (cb.x < 0.0 && ca.y < 0.0) ? -1.0 : 1.0;
  return s * sqrt(min(dot2(ca), dot2(cb)));
}

/**
 * 任意朝向的带端盖圆锥台
 * p: 待测试点
 * a: 底面中心
 * b: 顶面中心
 * ra: 底面半径（在 a 点）
 * rb: 顶面半径（在 b 点）
 */
float sdCappedCone(vec3 p, vec3 a, vec3 b, float ra, float rb)
{
  float rba = rb - ra;
  float baba = dot(b - a, b - a);
  float papa = dot(p - a, p - a);
  float paba = dot(p - a, b - a) / baba;

  float x = sqrt(papa - paba * paba * baba);

  float cax = max(0.0, x - ((paba < 0.5) ? ra : rb));
  float cay = abs(paba - 0.5) - 0.5;

  float k = rba * rba + baba;
  float f = clamp((rba * (x - ra) + paba * baba) / k, 0.0, 1.0);

  float cbx = x - ra - f * rba;
  float cby = paba - f;

  float s = (cbx < 0.0 && cay < 0.0) ? -1.0 : 1.0;

  return s * sqrt(min(cax * cax + cay * cay * baba, cbx * cbx + cby * cby * baba));
}

/**
 * 实心角（solid angle）：由原点出发、张角由 (c.x, c.y) 定义的无限锥体
 * p: 待测试点
 * c: vec2(sinθ, cosθ)，定义锥体半角 θ
 * ra: 锥体被截断的球面半径（若 ra>0，则为有限锥台）
 */
// c is the sin/cos of the desired cone angle
float sdSolidAngle(vec3 pos, vec2 c, float ra)
{
  vec2 p = vec2(length(pos.xz), pos.y);
  float l = length(p) - ra;
  float m = length(p - c * clamp(dot(p, c), 0.0, ra));
  return max(l, m * sign(c.y * p.x - c.x * p.y));
}

/**
 * 正八面体（regular octahedron），顶点在坐标轴上
 * p: 待测试点
 * s: 从中心到每个面的距离（即“内切球”半径）
 */
float sdOctahedron(vec3 p, float s)
{
  p = abs(p);
  float m = p.x + p.y + p.z - s;

// exact distance
#if 0
    vec3 o = min(3.0*p - m, 0.0);
    o = max(6.0*p - m*2.0 - o*3.0 + (o.x+o.y+o.z), 0.0);
    return length(p - s*o/(o.x+o.y+o.z));
#endif

// exact distance
#if 1
  vec3 q;
  if (3.0 * p.x < m)
    q = p.xyz;
  else if (3.0 * p.y < m)
    q = p.yzx;
  else if (3.0 * p.z < m)
    q = p.zxy;
  else
    return m * 0.57735027;
  float k = clamp(0.5 * (q.z - q.y + s), 0.0, s);
  return length(vec3(q.x, q.y - s + k, q.z - k));
#endif

// bound, not exact
#if 0
	return m*0.57735027;
#endif
}

/**
 * 四棱锥（square pyramid），底面为 1×1 正方形，顶点在 (0,h,0)
 * p: 待测试点
 * h: 金字塔高度
 */
float sdPyramid(in vec3 p, in float h)
{
  float m2 = h * h + 0.25;

  // symmetry
  p.xz = abs(p.xz);
  p.xz = (p.z > p.x) ? p.zx : p.xz;
  p.xz -= 0.5;

  // project into face plane (2D)
  vec3 q = vec3(p.z, h * p.y - 0.5 * p.x, h * p.x + 0.5 * p.y);

  float s = max(-q.x, 0.0);
  float t = clamp((q.y - 0.5 * p.z) / (m2 + 0.25), 0.0, 1.0);

  float a = m2 * (q.x + s) * (q.x + s) + q.y * q.y;
  float b = m2 * (q.x + 0.5 * t) * (q.x + 0.5 * t) + (q.y - m2 * t) * (q.y - m2 * t);

  float d2 = min(q.y, -q.x * m2 - q.y * 0.5) > 0.0 ? 0.0 : min(a, b);

  // recover 3D and scale, and add sign
  return sqrt((d2 + q.z * q.z) / m2) * sign(max(q.z, -p.y));
  ;
}

/**
 * 菱形柱体（rhombus prism），底面为菱形，沿 Z 轴拉伸
 * p: 待测试点
 * la: 菱形 X 方向半轴长
 * lb: 菱形 Z 方向半轴长
 * h: 柱体半高
 * ra: 菱形边角倒圆半径（若为0则为尖角）
 */
// la,lb=semi axis, h=height, ra=corner
float sdRhombus(vec3 p, float la, float lb, float h, float ra)
{
  p = abs(p);
  vec2 b = vec2(la, lb);
  float f = clamp((ndot(b, b - 2.0 * p.xz)) / dot(b, b), -1.0, 1.0);
  vec2 q = vec2(
    length(p.xz - 0.5 * b * vec2(1.0 - f, 1.0 + f)) * sign(p.x * b.y + p.z * b.x - b.x * b.y) - ra,
    p.y - h);
  return min(max(q.x, q.y), 0.0) + length(max(q, 0.0));
}

/**
 * 马蹄铁形状（horseshoe），由圆弧和两臂组成
 * p: 待测试点
 * c: vec2(cosθ, sinθ)，定义开口角度（通常 θ≈π/2）
 * r: 圆弧半径
 * le: 臂长（从圆弧末端到端面）
 * w: vec2(厚度, 高度)/2，即半尺寸
 */
float sdHorseshoe(in vec3 p, in vec2 c, in float r, in float le, vec2 w)
{
  p.x = abs(p.x);
  float l = length(p.xy);
  p.xy = mat2(-c.x, c.y, c.y, c.x) * p.xy;
  p.xy = vec2((p.y > 0.0 || p.x > 0.0) ? p.x : l * sign(-c.x), (p.x > 0.0) ? p.y : l);
  p.xy = vec2(p.x, abs(p.y - r)) - vec2(le, 0.0);

  vec2 q = vec2(length(max(p.xy, 0.0)) + min(0.0, max(p.x, p.y)), p.z);
  vec2 d = abs(q) - w;
  return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

/**
 * U 形槽（U-channel）
 * p: 待测试点
 * r: U 形底部圆角半径
 * le: U 形臂长（从底部到端面）
 * w: vec2(厚度, 高度)/2，即半尺寸
 */
float sdU(in vec3 p, in float r, in float le, vec2 w)
{
  p.x = (p.y > 0.0) ? abs(p.x) : length(p.xy);
  p.x = abs(p.x - r);
  p.y = p.y - le;
  float k = max(p.x, p.y);
  vec2 q = vec2((k < 0.0) ? -k : length(max(p.xy, 0.0)), abs(p.z)) - w;
  return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0);
}

#endif // SHAPE_SDF_GLSL