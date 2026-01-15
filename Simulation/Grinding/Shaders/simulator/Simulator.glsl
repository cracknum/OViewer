#define MAX_STEPS 128
#define MAX_DIST 100.0
#define SURFACE_EPS 0.001

struct Sphere
{
  vec3 position;
  float radius;
};
struct Ray
{
  vec3 rayOrigin;
  vec3 rayDir;
};
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

float sceneSDF(vec3 p)
{
  float sphere = sdSphere(p, 0.5);
  float box = sdBox(p, vec3(0.3));
  return min(sphere, box);
}

float intersectSphere(Ray ray, Sphere sphere)
{
  vec3 m = ray.rayOrigin - sphere.position;
  float rds = dot(ray.rayDir, m);
  float d = pow(rds, 2.0) - dot(m, m) + sphere.radius * sphere.radius;
  if (d < 0.0f)
  {
    return -1.0f;
  }

  return -rds - sqrt(d);
}

float refineRoot(vec3 ro, vec3 rd, float ta, float tb)
{
  for (int i = 0; i < 8; i++)
  {                             // 迭代 8 次（可调）
    float tm = 0.5 * (ta + tb); // 中点 t
    vec3 pm = ro + tm * rd;     // 中点空间位置
    if (sceneSDF(pm) > 0.0)     // 如果中点在物体外部
      ta = tm;                  // 零点在 [tm, tb] → 左端点右移
    else
      tb = tm; // 零点在 [ta, tm] → 右端点左移
  }
  return 0.5 * (ta + tb); // 返回最终中点作为根的近似
}

float rayCastSDF(Ray ray, out float tIn, out float tOut)
{
  float t = 0.0;
  float prevDist = sceneSDF(ray.rayOrigin);
  bool isInside = (prevDist <= 0.0);
  tIn = -1.0;
  tOut = -1.0;
  bool foundEntry = false;

  for (int i = 0; i < MAX_STEPS; i++)
  {
    if (t > MAX_DIST)
      break;

    vec3 p = ray.rayOrigin + t * ray.rayDir;
    float dist = sceneSDF(p);

    if (!foundEntry && !isInside && dist <= 0.0)
    {
      tIn = refineRoot(ray.rayOrigin, ray.rayDir, t - prevDist, t);
      foundEntry = true;
      isInside = true;
    }
    else if (foundEntry && isInside && dist >= 0.0)
    {
      tOut = refineRoot(ray.rayOrigin, ray.rayDir, t - prevDist, t);
      return 1.0;
    }

    prevDist = dist;
    float stepSize = max(abs(dist), 0.01);
    float prevDist = stepSize;
    t += prevDist;
  }

  if (!foundEntry && isInside)
  {
    tIn = 0.0;
    for (int i = 0; i < MAX_STEPS; i++)
    {
      if (t > MAX_DIST)
        break;

      vec3 p = ray.rayOrigin + t * ray.rayDir;
      float dist = sceneSDF(p);
      if (dist > 0.0)
      {
        tOut = refineRoot(ray.rayOrigin, ray.rayDir, t - prevDist, t);
        return 1.0;
      }
      float stepSize = max(abs(dist), 0.01);
      float prevDist = stepSize;
      t += prevDist;
    }
  }

  return -1.0;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
  vec2 uv = (fragCoord / iResolution.xy) * 2.0f - 1.0f;

  vec3 rayTarget = vec3(0.0f);
  vec3 rayOrigin = vec3(0.0f, 0.0f, 2.0f);
  vec3 rayDir = vec3(uv, -1.0f);
  Sphere sphere;
  sphere.position = vec3(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.4f;
  Ray ray;
  ray.rayOrigin = rayOrigin;
  ray.rayDir = normalize(rayDir);
  float tEntry, tExit;
  if (rayCastSDF(ray, tEntry, tExit) > 0.0)
  {
    // 可视化 entry 和 exit 的深度
    float depth = (tEntry + tExit) * 0.5 / 3.0;
    fragColor = vec4(vec3(depth), 1.0);
  }
  else
  {
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}