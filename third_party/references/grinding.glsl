--- Fragment Shader ---

#version 430 core

//YHQ MergeTool fs
in vec2 texCoord;

layout(r32f, binding = 0) uniform image3D texOld;

layout(r32f, binding = 1) uniform image3D texNow;

uniform float maxf;



void main()

{

   ivec3 voxelCoord = ivec3(gl_FragCoord.x, gl_FragCoord.y, gl_Layer);

   float odist = imageLoad(texOld, voxelCoord).x;

   float ndist = imageLoad(texNow, voxelCoord).x;

   if (ndist < odist) {

       imageStore(texOld, voxelCoord, vec4(ndist, 0, 0, 0));

   }

   imageStore(texNow, voxelCoord, vec4(maxf, 0, 0, 0));



}
--- Fragment Shader ---

#version 430 core

//YHQ AB fs

#define attribute in

#define varying out

// Template for the polydata mappers fragment shader



uniform int PrimitiveIDOffset;



// VC position of this fragment

in vec4 vertexVCVSOutput;

in vec4 worldPos;



// A-buffer节点结构 (16字节对齐)

struct ABufferNode {

    vec4 color;    vec3 pos;      // 12 bytes

    float depth;     // 4 bytes

    uint next;       // 4 bytes

};



// 全局原子计数器

layout(binding = 0, offset = 0) uniform atomic_uint nodeCounter;



// A-buffer存储缓冲区

layout(std430, binding = 0) buffer ABufferStorage {

    ABufferNode nodes[];

};



// 每像素链表头指针

layout(r32ui, binding = 0) uniform uimage2D headPointers;



uniform ivec2 windowSize;

uniform int maxLayers;





// optional color passed in from the vertex shader, vertexColor

uniform float ambientIntensity; // the material ambient

uniform float diffuseIntensity; // the material diffuse

uniform float opacityUniform; // the fragment opacity

uniform vec3 ambientColorUniform; // ambient color

uniform vec3 diffuseColorUniform; // diffuse color

uniform float specularIntensity; // the material specular intensity

uniform vec3 specularColorUniform; // intensity weighted color

uniform float specularPowerUniform;





// optional surface normal declaration

uniform int cameraParallel;



// extra lighting parameters

uniform vec3 lightColor0;

uniform vec3 lightDirectionVC0; // normalized

uniform vec3 lightPositionVC0;

uniform vec3 lightAttenuation0;

uniform float lightConeAngle0;

uniform float lightExponent0;

uniform int lightPositional0;uniform vec3 lightColor1;

uniform vec3 lightDirectionVC1; // normalized

uniform vec3 lightPositionVC1;

uniform vec3 lightAttenuation1;

uniform float lightConeAngle1;

uniform float lightExponent1;

uniform int lightPositional1;



// Texture maps

//VTK::TMap::Dec



// Texture coordinates

//VTK::TCoord::Dec



// picking support

//VTK::Picking::Dec



// Depth Peeling Support

//VTK::DepthPeeling::Dec



// clipping plane vars

//VTK::Clip::Dec



// the output of this shader

out vec4 fragOutput0;





// Apple Bug

//VTK::PrimID::Dec



// handle coincident offsets

//VTK::Coincident::Dec



// Value raster

//VTK::ValuePass::Dec



void main()

{

  // VC position of this fragment. This should not branch/return/discard.

  vec4 vertexVC = vertexVCVSOutput;



  // Place any calls that require uniform flow (e.g. dFdx) here.

  vec3 fdx = vec3(dFdx(vertexVC.x),dFdx(vertexVC.y),dFdx(vertexVC.z));

  vec3 fdy = vec3(dFdy(vertexVC.x),dFdy(vertexVC.y),dFdy(vertexVC.z));

  //VTK::UniformFlow::Impl





  // Set gl_FragDepth here (gl_FragCoord.z by default)

  //VTK::Depth::Impl



  // Early depth peeling abort:

  //VTK::DepthPeeling::PreColor



  // Apple Bug

  //VTK::PrimID::Impl



  //VTK::Clip::Impl



  //VTK::ValuePass::Impl



    vec3 specularColor = specularIntensity * specularColorUniform;

  float specularPower = specularPowerUniform;

  vec3 ambientColor = ambientIntensity * ambientColorUniform;

  vec3 diffuseColor = diffuseIntensity * diffuseColorUniform;

  float opacity = opacityUniform;





  // Generate the normal if we are not passed in one

  fdx = normalize(fdx);

  fdy = normalize(fdy);

  vec3 normalVCVSOutput = normalize(cross(fdx,fdy));

  if (cameraParallel == 1 && normalVCVSOutput.z < 0.0) { normalVCVSOutput = -1.0*normalVCVSOutput; }

  if (cameraParallel == 0 && dot(normalVCVSOutput,vertexVC.xyz) > 0.0) { normalVCVSOutput = -1.0*normalVCVSOutput; }



    vec3 diffuse = vec3(0,0,0);

  vec3 specular = vec3(0,0,0);

  vec
[2025-12-30 08:08:21.280] [26936] [INFO ] [d:\project\csurgery\master\apps\ntsurgerysystem\src\ntigniter.cpp:112|NTIgniter::OnReadProcessStandardOutput] 3 vertLightDirectionVC;

  float attenuation;

  float df;

  float sf;

    attenuation = 1.0;

    if (lightPositional0 == 0) {

      vertLightDirectionVC = lightDirectionVC0; }

    else {

      vertLightDirectionVC = vertexVC.xyz - lightPositionVC0;

      float distanceVC = length(vertLightDirectionVC);

      vertLightDirectionVC = normalize(vertLightDirectionVC);

      attenuation = 1.0 /

        (lightAttenuation0.x

         + lightAttenuation0.y * distanceVC

         + lightAttenuation0.z * distanceVC * distanceVC);

      // per OpenGL standard cone angle is 90 or less for a spot light

      if (lightConeAngle0 <= 90.0) {

        float coneDot = dot(vertLightDirectionVC, lightDirectionVC0);

        // if inside the cone

        if (coneDot >= cos(radians(lightConeAngle0))) {

          attenuation = attenuation * pow(coneDot, lightExponent0); }

        else {

          attenuation = 0.0; }

        }

      }

    df = max(0.0,attenuation*dot(normalVCVSOutput, -vertLightDirectionVC));

    diffuse += (df * lightColor0);

    sf = sign(df)*attenuation*pow( max(0.0, dot( reflect(vertLightDirectionVC, normalVCVSOutput), normalize(-vertexVC.xyz))), specularPower);

      specular += (sf * lightColor0);

    attenuation = 1.0;

    if (lightPositional1 == 0) {

      vertLightDirectionVC = lightDirectionVC1; }

    else {

      vertLightDirectionVC = vertexVC.xyz - lightPositionVC1;

      float distanceVC = length(vertLightDirectionVC);

      vertLightDirectionVC = normalize(vertLightDirectionVC);

      attenuation = 1.0 /

        (lightAttenuation1.x

         + lightAttenuation1.y * distanceVC

         + lightAttenuation1.z * distanceVC * distanceVC);

      // per OpenGL standard cone angle is 90 or less for a spot light

      if (lightConeAngle1 <= 90.0) {

        float coneDot = dot(vertLightDirectionVC, lightDirectionVC1);

        // if inside the cone

        if (coneDot >= cos(radians(lightConeAngle1))) {

          attenuation = attenuation * pow(coneDot, lightExponent1); }

        else {

          attenuation = 0.0; }

        }

      }

    df = max(0.0,attenuation*dot(normalVCVSOutput, -vertLightDirectionVC));

    diffuse += (df * lightColor1);

    sf = sign(df)*attenuation*pow( max(0.0, dot( reflect(vertLightDirectionVC, normalVCVSOutput), normalize(-vertexVC.xyz))), specularPower);

      specular += (sf * lightColor1);

  diffuse = diffuse * diffuseColor;

  specular = specular * specularColor;

  fragOutput0 = vec4(ambientColor + diffuse + specular, opacity);  //VTK::Light::Impl



  ivec2 coord = ivec2(gl_FragCoord.xy);



    // 检查像素坐标是否有效

    if (coord.x < 0 || coord.x >= windowSize.x ||

        coord.y < 0 || coord.y >= windowSize.y) {

        discard;

    }



    // 分配新节点索引

    uint newNodeIndex = atomicCounterIncrement(nodeCounter);

    uint maxNodes = uint(windowSize.x) * uint(windowSize.y) * uint(maxLayers);



    if (newNodeIndex >= maxNodes) {

        // 缓冲区已满，丢弃片元

        discard;

        return;

    }



    // 存储片元数据到新节点

    nodes[newNodeIndex].color = vec4(ambientColor + diffuse + specular, opacity);

    nodes[newNodeIndex].pos = worldPos.xyz;

    nodes[newNodeIndex].depth = gl_FragCoord.z;



    // 将新节点插入链表头部

    uint previousHead = imageAtomicExchange(headPointers, coord, newNodeIndex);

    nodes[newNodeIndex].next = previousHead;

   fragOutput0.a = 0;







  if (fragOutput0.a <= 0.0)

    {

    discard;

    }



  //VTK::DepthPeeling::Impl



  //VTK::Picking::Impl



  // handle coincident offsets

  //VTK::Coincident::Impl

}







===============================================



================ SHADER PROGRAM ================

Program ptr: 00000212887A2910



--- Fragment Shader ---

#version 430 core

//YHQ Composite fs



out vec4 fragOutput0;



in vec2 texCoord;

struct ABufferNode {

   vec4 color;   vec3 pos;

   float depth;

   uint next;

};



layout(std430, binding = 0) buffer ABufferStorage {

    ABufferNode nodes[];

};



layout(r32ui, binding = 0) uniform uimage2D headPointers;



uniform sampler2D opaqueDepthTex;

uniform ivec2 windowSize;

uniform int maxLayers;

uniform sampler3D toolOldTex;

uniform sampler3D toolNowTex;

uniform vec3 gridOrigin;

uniform vec3 gridSpacing;

uniform vec3 gridSize;



uniform vec3 grindBaseClr;

uniform float grindResectPos;

uniform vec3 grindResectClr;

uniform vec3 grindExpandDefaultClr;

uniform mat4 grindMatrix;

uniform float expandLen0;

uniform vec3 expandClr0;

uniform vec4 grindGeomSize;



float sdSphere(in vec3 p, in vec4 checkSize) {

    vec3 o = (grindMatrix * vec4(p, 1)).xyz;

    return length(o.xyz) - checkSize.x;

}





uniform float ambientIntensity; // the material ambient

uniform float diffuseIntensity; // the material diffuse

uniform float specularIntensity; // the material specular intensity

uniform float specularPowerUniform;



uniform vec3 lightColor0;

uniform vec3 lightDirectionVC0; // normalized

uniform vec3 lightPositionVC0;

uniform vec3 lightAttenuation0;

uniform float lightConeAngle0;

uniform float lightExponent0;

uniform int lightPositional0;



uniform vec3 lightColor1;

uniform vec3 lightDirectionVC1; // normalized

uniform vec3 lightPositionVC1;

uniform vec3 lightAttenuation1;

uniform float lightConeAngle1;

uniform float lightExponent1;

uniform int lightPositional1;





uniform int cameraParallel;

uniform mat3 normalMatrix;

uniform mat4 wcvcMatrix;

// 插入排序函数（按深度从近到远排序）

void sortNodes(inout ABufferNode fragments[64], int count) {

   for (int i = 1; i < count; i++) {

       ABufferNode key = fragments[i];

       int j = i - 1;

       // 按深度从大到小排序（近到远）

       while (j >= 0 && fragments[j].depth > key.depth) {

           fragments[j + 1] = fragments[j];

           j = j - 1;

       }

       fragments[j + 1] = key;

   }

}



vec3 worldPosTotexCoord(vec3 worldPos) {

   vec3 voxelCoords = ((worldPos - gridOrigin) / gridSpacing) + 0.5;

   vec3 texCoords = voxelCoords / gridSize;

   return clamp(texCoords, 0.0, 1.0);

}



float sampleOpaqueDepth() {

    return texture(opaqueDepthTex, texCoord).r;

}

vec3 getCutColor(vec3 worldPos, bool isface){

   vec3 clr;

       if (isface) {

        clr = grindBaseClr;

    } else {

        clr = grindExpandDefaultClr;

    }

    float dist = sdSphere(worldPos, grindGeomSize);

    if (dist <= grindResectPos) {

        clr = grindResectClr;

    } else {

        do {

            if (isface) {

                break;

            }

            if (dist <= expandLen0 + grindResectPos) {

                clr = expandClr0;

                break;

            }

        } while(false);

    }





   return clr;

}



float sampleSDF(sampler3D tex, vec3 wp) {

    vec3 tp = worldPosTotexCoord(wp);

    return texture(tex, tp).r;

}

vec3 computeOptimizedSobel(sampler3D tex, vec3 wp) {

    vec3 dt = 0.5 / gridSize;

    vec3 tc = worldPosTotexCoord(wp);

    // axis-aligned (正反)

    float x1 = texture(tex, tc + vec3( dt.x, 0, 0)).r;

    float x2 = texture(tex, tc - vec3( dt.x, 0, 0)).r;

    float y1 = texture(tex, tc + vec3(0,  dt.y, 0)).r;

    float y2 = texture(tex, tc - vec3(0,  dt.y, 0)).r;

    float z1 = texture(tex, tc + vec3(0, 0,  dt.z)).r;

    float z2 = texture(tex, tc - vec3(0, 0,  dt.z)).r;

    // diagonal samples（8 个最优对角样本）

    float dpxy = texture(tex, tc + vec3( dt.x,  dt.y, 0)).r;

    float dpxy2= texture(tex, tc - vec3( dt.x,  dt.y, 0)).r;

    float dpxz = texture(tex, tc + vec3( dt.x, 0,  dt.z)).r;

    float dpxz2= texture(tex, tc - vec3( dt.x, 0,  dt.z)).r;

    float dpyz = texture(tex, tc + vec3(0,  dt.y,  dt.z)).r;

    float dpyz2= texture(tex, tc - vec3(0,  dt.y,  dt.z)).r;

    float gx = (x1 - x2) +

               0.5 * ((dpxy - dpxy2) + (dpxz - dpxz2));

    float gy = (y1 - y2) +

               0.5 * ((dpxy - dpxy2) + (dpyz - dpyz2));

    float gz = (z1 - z2) +

               0.5 * ((dpxz - dpxz2) + (dpyz - dpyz2));

    vec3 g = vec3(gx, gy, gz) / gridSpacing;

    return normalize(g);

}

const int MAX_STEPS = 1024;

#define REFINE_ITERS 5

bool raymarch(sampler3D tex, vec3 ro, vec3 rd, int starts, float startd, float maxd, out vec3 hitp, out vec3 hitn, out int hits, out float hitd) {

    float d = startd;

    float prevD   = d;

    float prevSDF = sampleSDF(tex, ro + rd * d);

    for (int i = starts; i < MAX_STEPS && d < maxd; ++i) {

        vec3 p = ro + rd * d;

        float sdf = sampleSDF(tex, p);

        if (sdf > -0.0001) {

            float t0 = prevD;

            float t1 = d;

            float s0 = prevSDF;

            float s1 = sdf;

            if (s0 * s1 < 0.0) {

                float minS = abs(s0);

                float minT = t0;

                if (abs(s1) < abs(s0)) {

                    minS = abs(s1);

                    minT = t1;

                }                for (int k = 0; k < REFINE_ITERS; ++k) {

                    float denom = (s1 - s0);

                    if (abs(denom) < 1e-6)

                        break;

                    float t = (t0 * s1 - t1 * s0) / denom;

                    vec3  pp = ro + rd * t;

                    float ss = sampleSDF(tex, pp);

                    if (ss < 0.0) {

                        t0 = t; s0 = ss;

                    } else {

                        t1 = t; s1 = ss;

                    }

                    if (abs(ss) < minS) {

                        minS = abs(ss);

                        minT = t;

                    }                }

                float t = 0.5 * (t0 + t1);

                vec3  pp = ro + rd * t;

                float ss = sampleSDF(tex, pp);

                if (abs(ss) < minS) {

                    minS = abs(ss);

                    minT = t;

                }                hitd = minT;

            } else {

                hitd = t1;

            }

            hitp = ro + rd * hitd;

            hitn = normalize(computeOptimizedSobel(tex, hitp));

            if (dot(hitn, rd) > 0.0)

                hitn = -hitn;

            hits = i + 1;

            return true;

        }

        prevD = d;

        prevSDF = sdf;

        float stepLen = abs(sdf);

        d += stepLen;

    }

    return false;

}

bool raymarch_all(sampler3D texA, sampler3D texB, bool oface, vec3 ro, vec3 rd, float maxd, out vec3 hitp, out vec3 hitn, out float hitd, out bool face) {

    face = oface;

    int hits = 0; 

    while(true){

        if (!raymarch(texA, ro, rd, hits, hitd, maxd, hitp, hitn, hits, hitd)) {

            return false;

        }

        float nd = sampleSDF(texB, hitp);

        if (nd >= 0){

            return true;

        } 

        face = !face;

        if (!raymarch(texB, ro, rd, hits, hitd, maxd, hitp, hitn, hits, hitd)) {

            return false;

        }

        nd = sampleSDF(texA, hitp);

        if (nd >= 0){

            return true;

        } 

        face = !face;

    }

}



void main()

{

   ivec2 coord = ivec2(gl_FragCoord.xy);

   // 检查像素坐标是否有效

   if (coord.x < 0 || coord.x >= windowSize.x ||

       coord.y < 0 || coord.y >= windowSize.y) {

       discard;

       return;

   }

   // 收集该像素的所有片元

   ABufferNode fragments[64];

   int fragmentCount = 0;

   uint currentNode = imageLoad(headPointers, coord).r;

   // 遍历链表，收集片元

   while (currentNode != 0xFFFFFFFF && fragmentCount < maxLayers) {

       fragments[fragmentCount] = nodes[currentNode];

       currentNode = fragments[fragmentCount].next;

       fragmentCount++;

   }

   // 如果没有片元，直接丢弃

   if (fragmentCount == 0) {

       discard;

       return;

   }

   // 对片元按深度排序（从近到远）

   if (fragmentCount > 1) {

       sortNodes(fragments, fragmentCount);

   }

   if (fragmentCount == 1) {fragOutput0 = vec4(1,0,0,1); return;}

   if (fragmentCount == 3) {fragOutput0 = vec4(0,1,0,1); return;}

   if (fragmentCount == 5) {fragOutput0 = vec4(0,0,1,1); return;}

   if (fragmentCount == 7) {fragOutput0 = vec4(1,0,1,1); return;}

   if (fragmentCount == 9) {fragOutput0 = vec4(1,1,0,1); return;}

   if (fragmentCount == 11) {fragOutput0 = vec4(0,1,1,1); return;}

   if (fragmentCount == 13) {fragOutput0 = vec4(1,0.5,0,1); return;}

   if (fragmentCount == 15) {fragOutput0 = vec4(0.5,0,1,1); return;}

   for (int i = 0; i < fragmentCount; i += 2) {

       vec3 start = fragments[i].pos;

       float opaqueDepth = sampleOpaqueDepth();

       if (opaqueDepth < fragments[i].depth) {

           gl_FragDepth = opaqueDepth;

           return;

       }

       float od = sampleSDF(toolOldTex, start);

       float nd = sampleSDF(toolNowTex, start);

       if (od >= 0 && nd >= 0) {

           gl_FragDepth = fragments[i].depth;

           fragOutput0 = fragments[i].color * vec4(getCutColor(start, true), 1);

           return;

       }

       vec3 end = fragments[i + 1].pos;

       vec3 rd = normalize(end - start);

       float maxd = distance(end, start);

       float sdepth = fragments[i].depth;

       float tdepth = fragments[i+1].depth - sdepth;

       vec3 hitp, hitn;

       float hitd = 0;

       bool face = false;

       if (od < 0) {

           if (raymarch_all(toolOldTex,toolNowTex, true, start, rd, maxd, hitp, hitn, hitd, face)) {

               float ndepth = sdepth + tdepth * hitd / maxd;

               if (opaqueDepth < ndepth) {

                   gl_FragDepth = opaqueDepth;

                   return;

               }

               vec3 cutColor = getCutColor(hitp, face);

               vec4 vertexVC = wcvcMatrix * vec4(hitp, 1);

               vec3 normal = normalMatrix * hitn;

               if (cameraParallel == 1 && normal.z < 0.0) { normal = -1.0*normal; }

               if (cameraParallel == 0 && dot(normal,vertexVC.xyz) > 0.0) { normal = -1.0*normal; }

                 vec3 specularColor = specularIntensity * cutColor;

  float specularPower = specularPowerUniform;

  vec3 ambientColor = ambientIntensity * cutColor;

  vec3 diffuseColor = diffuseIntensity * cutColor;

  float opacity = 1;



                 vec3 diffuse = vec3(0,0,0);

  vec3 specular = vec3(0,0,0);

  vec3 vertLightDirectionVC;

  float attenuation;

  float df;

  float sf;

    attenuation = 1.0;

    if (lightPositional0 == 0) {

      vertLightDirectionVC = lightDirectionVC0; }

    else {

      vertLightDirectionVC = vertexVC.xyz - lightPositionVC0;

      float distanceVC = length(vertLightDirectionVC);

      vertLightDirectionVC = normalize(vertLightDirectionVC);

      attenuation = 1.0 /

        (lightAttenuation0.x

         + lightAttenuation0.y * distanceVC

         + lightAttenuation0.z * distanceVC * distanceVC);

      // per OpenGL standard cone angle is 90 or less for a spot light

      if (lightConeAngle0 <= 90.0) {

        float coneDot = dot(vertLightDirectionVC, lightDirectionVC0);

        // if inside the cone

        if (coneDot >= cos(radians(lightConeAngle0))) {

          attenuation = attenuation * pow(coneDot, lightExponent0); }

        else {

          attenuation = 0.0; }

        }

      }

    df = max(0.0,attenuation*dot(normal, -vertLightDirectionVC));

    diffuse += (df * lightColor0);

    sf = sign(df)*attenuation*pow( max(0.0, dot( reflect(vertLightDirectionVC, normal), normalize(-vertexVC.xyz))), specularPower);

      specular += (sf * lightColor0);

    attenuation = 1.0;

    if (lightPositional1 == 0) {

      vertLightDirectionVC = lightDirectionVC1; }

    else {

      vertLightDirectionVC = vertexVC.xyz - lightPositionVC1;

      float distanceVC = length(vertLightDirectionVC);

      vertLightDirectionVC = normalize(vertLightDirectionVC);

      attenuation = 1.0 /

        (lightAttenuation1.x

         + lightAttenuation1.y * distanceVC

         + lightAttenuation1.z * distanceVC * distanceVC);

      // per OpenGL standard cone angle is 90 or less for a spot light

      if (lightConeAngle1 <= 90.0) {

        float coneDot = dot(vertLightDirectionVC, lightDirectionVC1);

        // if inside the cone

        if (coneDot >= cos(radians(lightConeAngle1))) {

          attenuation = attenuation * pow(coneDot, lightExponent1); }

        else {

          attenuation = 0.0; }

        }

      }

    df = max(0.0,attenuation*dot(normal, -vertLightDirectionVC));

    diffuse += (df * lightColor1);

    sf = sign(df)*attenuation*pow( max(0.0, dot( reflect(vertLightDirectionVC, normal), normalize(-vertexVC.xyz))), specularPower);

      specular += (sf * lightColor1);

  diffuse = diffuse * diffuseColor;

  specular = specular * specularColor;

  fragOutput0 = vec4(ambientColor + diffuse + specular, opacity);

               gl_FragDepth = ndepth;

               return;

           } 

       } else { 

      
[2025-12-30 08:08:22.394] [26936] [INFO ] [d:\project\csurgery\master\apps\ntsurgerysystem\src\ntigniter.cpp:112|NTIgniter::OnReadProcessStandardOutput]      if (raymarch_all(toolNowTex, toolOldTex, false, start, rd, maxd, hitp, hitn, hitd, face)) {

               float ndepth = sdepth + tdepth * hitd / maxd;

               if (opaqueDepth < ndepth) {

                   gl_FragDepth = opaqueDepth;

                   return;

               }

               vec3 cutColor = getCutColor(hitp, face);

               vec4 vertexVC = wcvcMatrix * vec4(hitp, 1);

               vec3 normal = normalMatrix * hitn;

               if (cameraParallel == 1 && normal.z < 0.0) { normal = -1.0*normal; }

               if (cameraParallel == 0 && dot(normal,vertexVC.xyz) > 0.0) { normal = -1.0*normal; }

                 vec3 specularColor = specularIntensity * cutColor;

  float specularPower = specularPowerUniform;

  vec3 ambientColor = ambientIntensity * cutColor;

  vec3 diffuseColor = diffuseIntensity * cutColor;

  float opacity = 1;



                 vec3 diffuse = vec3(0,0,0);

  vec3 specular = vec3(0,0,0);

  vec3 vertLightDirectionVC;

  float attenuation;

  float df;

  float sf;

    attenuation = 1.0;

    if (lightPositional0 == 0) {

      vertLightDirectionVC = lightDirectionVC0; }

    else {

      vertLightDirectionVC = vertexVC.xyz - lightPositionVC0;

      float distanceVC = length(vertLightDirectionVC);

      vertLightDirectionVC = normalize(vertLightDirectionVC);

      attenuation = 1.0 /

        (lightAttenuation0.x

         + lightAttenuation0.y * distanceVC

         + lightAttenuation0.z * distanceVC * distanceVC);

      // per OpenGL standard cone angle is 90 or less for a spot light

      if (lightConeAngle0 <= 90.0) {

        float coneDot = dot(vertLightDirectionVC, lightDirectionVC0);

        // if inside the cone

        if (coneDot >= cos(radians(lightConeAngle0))) {

          attenuation = attenuation * pow(coneDot, lightExponent0); }

        else {

          attenuation = 0.0; }

        }

      }

    df = max(0.0,attenuation*dot(normal, -vertLightDirectionVC));

    diffuse += (df * lightColor0);

    sf = sign(df)*attenuation*pow( max(0.0, dot( reflect(vertLightDirectionVC, normal), normalize(-vertexVC.xyz))), specularPower);

      specular += (sf * lightColor0);

    attenuation = 1.0;

    if (lightPositional1 == 0) {

      vertLightDirectionVC = lightDirectionVC1; }

    else {

      vertLightDirectionVC = vertexVC.xyz - lightPositionVC1;

      float distanceVC = length(vertLightDirectionVC);

      vertLightDirectionVC = normalize(vertLightDirectionVC);

      attenuation = 1.0 /

        (lightAttenuation1.x

         + lightAttenuation1.y * distanceVC

         + lightAttenuation1.z * distanceVC * distanceVC);

      // per OpenGL standard cone angle is 90 or less for a spot light

      if (lightConeAngle1 <= 90.0) {

        float coneDot = dot(vertLightDirectionVC, lightDirectionVC1);

        // if inside the cone

        if (coneDot >= cos(radians(lightConeAngle1))) {

          attenuation = attenuation * pow(coneDot, lightExponent1); }

        else {

          attenuation = 0.0; }

        }

      }

    df = max(0.0,attenuation*dot(normal, -vertLightDirectionVC));

    diffuse += (df * lightColor1);

    sf = sign(df)*attenuation*pow( max(0.0, dot( reflect(vertLightDirectionVC, normal), normalize(-vertexVC.xyz))), specularPower);

      specular += (sf * lightColor1);

  diffuse = diffuse * diffuseColor;

  specular = specular * specularColor;

  fragOutput0 = vec4(ambientColor + diffuse + specular, opacity);

               gl_FragDepth = ndepth;

               return;

           }

       }

   }

   discard;



}





===============================================



================ SHADER PROGRAM ================

Program ptr: 00000212887A15D0



--- Fragment Shader ---

#version 430 core

//YHQ UpdateTool fs







in vec2 texCoord;

layout(r32f, binding = 0) uniform image3D texDist;

layout(std430, binding = 0) buffer PolygonBuffer

{

    vec2 PG_Points[];

};

uniform int PGP_Count;

uniform vec3 gridOrigin;

uniform vec3 gridSpacing;

uniform vec3 gridSize;



vec3 texCoordToworldPos(vec3 tc) {

   vec3 voxelCoords = tc * gridSize;

   vec3 worldPos = (voxelCoords - 0.5) * gridSpacing + gridOrigin;
[2025-12-30 08:08:32.144] [1404] [INFO ] [d:\project\csurgery\master\plugins\org.nt.navigation.tha\src\internal\thanavigationflowview.cpp:1573|THANavigationFlowView::slotAboutToShowSettingMenu] show settting menu: adminMode[0]
[2025-12-30 08:08:32.961] [1404] [INFO ] [d:\project\csurgery\master\plugins\org.nt.navigation.tha\src\internal\thanavigationflowview.cpp:1108|THANavigationFlowView::slotExit] start exit NTSurgerySystem
[2025-12-30 08:08:32.966] [1404] [INFO ] [d:\project\csurgery\master\modules\ntpurewidgets\src\ntmessagebox.cpp:137|NTMessageBox::question] MessageBox question: 閫€鍑虹▼搴�, 璇风‘璁ゅ彇鍑洪�ㄩ拤锛屾槸鍚︾‘瀹氶€€鍑虹▼搴忥紵
[2025-12-30 08:08:33.813] [1404] [INFO ] [d:\project\csurgery\master\plugins\org.nt.navigation.tha\src\internal\thanavigationflowview.cpp:1119|THANavigationFlowView::slotExit] exit confirmed
[2025-12-30 08:08:33.958] [1404] [INFO ] [d:\project\csurgery\master\plugins\org.nt.navigation.tha\src\internal\thanavigationflowview.cpp:1141|THANavigationFlowView::slotExit] ################THA Navigation End################
[2025-12-30 08:08:33.959] [26936] [INFO ] [d:\project\csurgery\master\apps\ntsurgerysystem\src\ntigniter.cpp:112|NTIgniter::OnReadProcessStandardOutput] 

   return worldPos;

}



//tool parameters

uniform int toolType;

uniform mat4 toolMatrix;

uniform vec3 toolSize;

float sdSphere(in vec3 p) {

    vec3 o = (toolMatrix * vec4(p, 1)).xyz;

    return length(o.xyz) - toolSize.x;

}

float sdSemiSphere(in vec3 p) {

    vec3 o = (toolMatrix * vec4(p, 1)).xyz;

    float r = toolSize.x;

    float d1 = length(o.xyz) - r;

    float d2 = -o.z;

    return max(d1, d2);

}

float sdBox(in vec3 p){

    vec3 o = (toolMatrix * vec4(p, 1)).xyz;

    vec3 b = toolSize;

    vec3 q = abs(o.xyz) - b;

    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);

}

float sdCylinder(in vec3 p) {

    vec3 o = (toolMatrix * vec4(p, 1)).xyz;

    float r = toolSize.x;

    float h = toolSize.y;

    vec2 d = abs(vec2(length(o.xy),o.z)) - vec2(r,h);

    return min(max(d.x,d.y),0.0) + length(max(d,0.0));

}

float sdCapsule(in vec3 p) {

   vec3 o = (toolMatrix * vec4(p, 1)).xyz;

   float r = toolSize.x;

   float h = toolSize.y;

   o.z -= clamp(o.z, -h, h);

   return length(o) - r;

}

float sdSegment2D(vec2 p, vec2 a, vec2 b) {

    vec2 pa = p - a;

    vec2 ba = b - a;

    float baba = dot(ba, ba);

    if (baba < 1e-8)

       return length(pa);

    float h = clamp(dot(pa, ba) / baba, 0.0, 1.0);

    return length(pa - ba * h);

}

float sdPolygon2D(vec2 p) {

    float d = 1e20;

    if (PGP_Count < 3)

        return d;

    bool inside = false;

    for (int i = 0; i < PGP_Count; ++i) {

        vec2 a = PG_Points[i];

        vec2 b = PG_Points[(i + 1) % PGP_Count];

        d = min(d, sdSegment2D(p, a, b));

        float py = p.y + 1e-6;

        bool cond1 = (a.y > py) != (b.y > py);

        if (cond1) {

            float t = (py - a.y) / (b.y - a.y);

            float x = mix(a.x, b.x, t);

            if (p.x < x)

                inside = !inside;

        }

    }

    return inside ? -d : d;

}

float sd2DAxisDilate(in vec3 p) {

    vec3 o = (toolMatrix * vec4(p, 1)).xyz;

    float height = toolSize.x;

    int axis = int(floor(toolSize.y+0.5));

    float d2 = 1e20;

    float dh = 1e20;

    if (0 == axis){

        d2 = sdPolygon2D(o.yz);

        dh = abs(o.x) - height;

    } else if (1 == axis){

        d2 = sdPolygon2D(o.xz);

        dh = abs(o.y) - height;

    } else if (2 == axis){

        d2 = sdPolygon2D(o.xy);

        dh = abs(o.z) - height;

    }

    return max(d2, dh);

}

float sdRound2DAxisDilate(in vec3 p) {

    vec3 o = (toolMatrix * vec4(p, 1)).xyz;

    float radius = toolSize.x;

    float height = toolSize.y;

    int axis = int(floor(toolSize.z+0.5));

    float d2 = 1e20;

    float dh = 1e20;

    if (0 == axis){

        d2 = sdPolygon2D(o.yz);

        dh = abs(o.x) - height;

    } else if (1 == axis){

        d2 = sdPolygon2D(o.xz);

        dh = abs(o.y) - height;

    } else if (2 == axis){

        d2 = sdPolygon2D(o.xy);

        dh = abs(o.z) - height;

    }

    return length(max(vec2(d2, dh), 0.0)) - radius + min(max(d2, dh), 0.0); 

}



void main()

{

   vec3 toolTexCoord = vec3(gl_FragCoord.x / gridSize.x, gl_FragCoord.y / gridSize.y, (gl_Layer + 0.5) / gridSize.z);

   ivec3 voxelCoord = ivec3(gl_FragCoord.x, gl_FragCoord.y, gl_Layer);

   float odist = imageLoad(texDist, voxelCoord).x;

   vec3 worldPos = texCoordToworldPos(toolTexCoord);

   float ndist = 0;

   if (toolType == 0) {

       ndist = sdSphere(worldPos);

   } else if (toolType == 1) {

       ndist = sdSemiSphere(worldPos);

   } else if (toolType == 2) {

       ndist = sdBox(worldPos);

   } else if (toolType == 3) {

       ndist = sdCylinder(worldPos);

   } else if (toolType == 4) {

       ndist = sdCapsule(worldPos);

   } else if (toolType == 5) {

       ndist = sd2DAxisDilate(worldPos);

   } else if (toolType == 6) {

       ndist = sdRound2DAxisDilate(worldPos);

   } else {

       discard;

       return;

   }

   if (ndist < odist) {

       imageStore(texDist, voxelCoord, vec4(ndist, 0, 0, 0));

   }



}



