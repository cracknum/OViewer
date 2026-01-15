#version 440 core

layout(r32ui, binding = 2) uniform uimage2D headPointerTexImage;

struct ABufferNode
{
  vec4 color;
  vec4 position;
  uint next;
};

layout(std430, binding = 0) buffer ABufferStorage
{
  ABufferNode nodes[];
};

// 假设工具的纹理是不透明纹理
// samplerxx和imagexx拥有独立的命名空间，所以可以使用重复的binding
// layout(binding = 0) uniform sampler3D opaqueDepthTex;
uniform sampler3D toolTex;

uniform ivec2 windowSize;
uniform int maxLayer;

uniform vec3 gridOrigin;
uniform vec3 gridSize;
uniform vec3 gridSpacing;

in vec2 texCoord;

#define RAY_MARCH_MAX_STEP 1024
#define REFINE_ITEMS 5
#ifndef USE_FEATURE_X
#define DEPTH_DEBUG 1
#endif

out vec4 fragColor;

void swapBufferNode(in ABufferNode bufferNode[64], int firstIndex, int secondIndex)
{
  ABufferNode node = bufferNode[firstIndex];
  bufferNode[firstIndex].color = bufferNode[secondIndex].color;
  bufferNode[firstIndex].position = bufferNode[secondIndex].position;
  bufferNode[firstIndex].next = bufferNode[secondIndex].next;

  bufferNode[secondIndex].color = node.color;
  bufferNode[secondIndex].position = node.position;
  bufferNode[secondIndex].next = node.next;
}
void sortFragments(inout ABufferNode bufferNode[64], int fragmentCount)
{
  for (int i = fragmentCount - 1; i > 0; i--)
  {
    bool swaped = false;
    for (int j = 0; j < i; j++)
    {
      if (bufferNode[j].next > bufferNode[j + 1].next)
      {
        swapBufferNode(bufferNode, j, j + 1);
        swaped = true;
      }
    }
    if (!swaped)
    {
      return;
    }
  }
}

vec3 worldToTexCoord(in vec3 worldPos)
{
   vec3 minCenter = gridOrigin + 0.5 * gridSpacing;
   vec3 maxCenter = gridOrigin + (gridSize - 0.5) * gridSpacing;
    
   vec3 t = (worldPos - minCenter) / (maxCenter - minCenter);
    
   return clamp(t, 0.0, 1.0);
}

float sampleSDF(in sampler3D tex, in vec3 worldPosition)
{
  vec3 texNormalizedCoord = worldToTexCoord(worldPosition);
  return texture(tex, texNormalizedCoord).r;
}

vec3 computeOptimizedSobel(in sampler3D tex, in vec3 worldPoint)
{
    vec3 dt = 0.5 / gridSize;
    vec3 texCoord = worldToTexCoord(worldPoint);

    float x1 = texture(tex, texCoord - vec3(dt.x, 0, 0)).r;
    float x2 = texture(tex, texCoord + vec3(dt.x, 0, 0)).r;

    float y1 = texture(tex, texCoord - vec3(0, dt.y, 0)).r;
    float y2 = texture(tex, texCoord + vec3(0, dt.y, 0)).r;

    float z1 = texture(tex, texCoord - vec3(0, 0, dt.z)).r;
    float z2 = texture(tex, texCoord + vec3(0, 0, dt.z)).r;

    float xy1 = texture(tex, texCoord - vec3(dt.x, dt.y, 0)).r;
    float xy2 = texture(tex, texCoord + vec3(dt.x, dt.y, 0)).r;

    float xz1 = texture(tex, texCoord - vec3(dt.x, 0, dt.z)).r;
    float xz2 = texture(tex, texCoord + vec3(dt.x, 0, dt.z)).r;

    float yz1 = texture(tex, texCoord - vec3(0, dt.y, dt.z)).r;
    float yz2 = texture(tex, texCoord + vec3(0, dt.y, dt.z)).r;

    float dx = (x1 - x2) + 0.5 * ((xy1 - xy2) + (xz1 - xz2));
    float dy = (y1 - y2) + 0.5 * ((xy1 - xy2) + (yz1 - yz2));
    float dz = (z1 - z2) + 0.5 * ((xz1 - xz2) + (yz1 - yz2));

    // 中心差分计算时的分母 gridSpacing = 2 * dt;
    return normalize(vec3(dx, dy, dz) / gridSpacing);
}

bool rayMarch(in sampler3D tex, vec3 ro, vec3 rd, in float maxd, in int startStep, in float startDist, out vec3 hitp, out vec3 hitn, out float hitd, out int hitStep, out bool face)
{
    float dist = startDist;
    float prevDist = startDist;
    float prevSDFValue = sampleSDF(tex, ro + rd * dist);

    for (int i = startStep; i < RAY_MARCH_MAX_STEP && dist < maxd; ++i)
    {
        vec3 marchPoint = ro + rd * dist;
        float sdfValue = sampleSDF(tex, marchPoint);

        if (sdfValue > -0.0001)
        {
            float t0 = prevDist;
            float s0 = prevSDFValue;
            float t1 = startDist;
            float s1 = sdfValue;

            if ( s0 * s1 < 0)
            {
                float minSDFValue = abs(s0);
                float minDist = t0;
                if(abs(s1) < abs(s0))
                {
                    minSDFValue = abs(s1);
                    minDist = t1;
                }

                for(int k = 0; k < REFINE_ITEMS; k++)
                {
                    float denom = (s1 - s0);
                    if (abs(denom) < 1e-6)
                    {
                        break;
                    }

                    float t = (t0 * s1 - t1 * s0) / denom;
                    vec3 refinePoint = ro + rd * t;
                    float refineSDFValue = sampleSDF(tex, refinePoint);
                    if (refineSDFValue < 0.0)
                    {
                        t0 = t;
                        s0 = refineSDFValue;
                    }
                    else
                    {
                        t1 = t;
                        s1 = refineSDFValue;
                    }

                    if (abs(refineSDFValue) < minSDFValue)
                    {
                        minSDFValue = abs(refineSDFValue);
                        minDist = t;
                    }
                }

                hitd = minDist;
            }
            else
            {
                hitd = dist;
            }

            hitp = ro + rd * hitd;
            hitn = computeOptimizedSobel(tex, hitp);

            if (dot(hitn, rd) > 0)
            {
                hitn = -hitn;
            }
            hitStep = i + 1;

            return true;
        }

        prevDist = dist;
        prevSDFValue = sdfValue;
        dist += abs(sdfValue);
    }

    return false;
}

void main()
{  
  ivec2 coord = ivec2(texCoord.xy);
  vec2 normalizedCoord = coord / windowSize;
  fragColor = vec4(normalizedCoord.x, 1.0, 0.5, 1.0);
//   ABufferNode fragments[64];
//   int fragmentCount = 0;
//   uint currentNode = imageLoad(headPointerTexImage, coord).r;

//   while (currentNode != 0xffffffff && fragmentCount < maxLayer)
//   {
//     fragments[fragmentCount] = nodes[currentNode];
//     currentNode = nodes[currentNode].next;
//     fragmentCount++;
//   }

// #if DEPTH_DEBUG
//   if (fragmentCount == 0)
//   {
//     fragColor = vec4(1, 1, 0, 1);
//     gl_FragDepth = 0;
//   }
//   else
//   {
//     fragColor = vec4(fragmentCount * 1.0 / 10, 0.0, 0.0, 1.0);
//     gl_FragDepth = fragments[0].position.z;
//   }
// #endif

//   if (fragmentCount == 0)
//   {
//     fragColor = vec4(0, 0, 0, 1);
//     gl_FragDepth = 0;
//     return;
//   }

//   sortFragments(fragments, fragmentCount);

//   for(int i = 0; i < fragmentCount; i += 2){
//     vec3 start = vec3(fragments[i].position.xyz);
//     vec3 end = vec3(fragments[i+1].position.xyz);
//     vec3 rayOrigin = start;
//     vec3 rayDirection = normalize(end - start);

//     float sdfValue = sampleSDF(toolTex, start);

//     if (sdfValue > 0)
//     {
//         fragColor = fragments[i].color;
//         gl_FragDepth = start.z;
//         return;
//     }

//     float maxDist = distance(end, start);
//     vec3 hitp, hitn;
//     float hitDist;
//     int hitStep;
//     bool face = false;

//     if (!rayMarch(toolTex, rayOrigin, rayDirection, maxDist, 0, 0, hitp, hitn, hitDist, hitStep, face))
//     {
//         continue;
//     }

//     float sdepth = fragments[i].position.z;
//     float tdepth = fragments[i+1].position.z - sdepth;
//     float ndepth = sdepth + tdepth * hitDist / maxDist;

//     fragColor = vec4(0.0, 1.0, 0.0, 1.0);
//     gl_FragDepth = ndepth;
//     break;
//   }
 }
