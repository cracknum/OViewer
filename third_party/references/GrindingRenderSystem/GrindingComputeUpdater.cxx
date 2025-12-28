#include "GrindingComputeUpdater.h"
#include <vtk_glew.h>
#include <vtkMatrix4x4.h>
#include <algorithm>
#include <vector>

static const char* ComputeSource = R"(
#version 430 core
#define MAX_TOOLS 16
layout(r32f, binding = 0) uniform image3D removalVol;
layout(std430, binding = 0) buffer ToolParams {
    int numTools;
    vec3 volumeMin;
    vec3 voxelSize;
    vec4 toolData[];
};
vec3 worldToToolLocal(vec3 worldPos, int toolIdx)
{
    int base = toolIdx * 4;
    mat3 rot = mat3(toolData[base + 1].xyz, toolData[base + 2].xyz, toolData[base + 3].xyz);
    vec3 trans = vec3(toolData[base + 1].w, toolData[base + 2].w, toolData[base + 3].w);
    return rot * (worldPos - trans);
}
float evalToolSDF(vec3 worldPos, int toolIdx)
{
    int base = toolIdx * 4;
    int type = int(toolData[base].x);
    float p0 = toolData[base].y;
    float p1 = toolData[base].z;
    vec3 local = worldToToolLocal(worldPos, toolIdx);
    if (type == 0) return local.z - p0;
    else if (type == 1) {
        float radial = length(local.xy);
        float dz = abs(local.z) - p1 * 0.5;
        if (radial <= p0 && dz <= 0.0) return -1.0;
        return min(radial - p0, dz);
    }
    else if (type == 2) return length(local) - p0;
    return 1e6;
}
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);
    ivec3 dims = imageSize(removalVol);
    if (any(greaterThanEqual(gid, dims))) return;
    float current = imageLoad(removalVol, gid).r;
    if (current > 0.5) return;
    vec3 worldPos = volumeMin + (vec3(gid) + 0.5) * voxelSize;
    bool removed = false;
    for (int i = 0; i < numTools && i < MAX_TOOLS; ++i)
    {
        if (evalToolSDF(worldPos, i) <= 0.0)
        {
            removed = true;
            break;
        }
    }
    if (removed) imageStore(removalVol, gid, vec4(1.0));
}
)";

GrindingComputeUpdater::GrindingComputeUpdater(vtkOpenGLRenderWindow* renWin)
  : RenderWindow(renWin)
{
  this->CreateShader();
  glGenBuffers(1, &this->ToolSSBO);
}

GrindingComputeUpdater::~GrindingComputeUpdater()
{
  if (this->ComputeProgram) glDeleteProgram(this->ComputeProgram);
  if (this->ToolSSBO) glDeleteBuffers(1, &this->ToolSSBO);
}

void GrindingComputeUpdater::CreateShader()
{
  GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(shader, 1, &ComputeSource, nullptr);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    GLchar log[1024];
    glGetShaderInfoLog(shader, 1024, nullptr, log);
    return;
  }

  this->ComputeProgram = glCreateProgram();
  glAttachShader(this->ComputeProgram, shader);
  glLinkProgram(this->ComputeProgram);

  GLint linked;
  glGetProgramiv(this->ComputeProgram, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    GLchar log[1024];
    glGetProgramInfoLog(this->ComputeProgram, 1024, nullptr, log);
  }
}

void GrindingComputeUpdater::UploadToolData(
  const std::vector<ToolParams>& tools,
  const double volumeMin[3],
  const double volumeMax[3])
{
  int num = std::min((size_t)MAX_TOOLS, tools.size());
  std::vector<float> ssboData;
  ssboData.resize(12 + num * 16); // header(12) + tools

  ssboData[0] = (float)num;
  ssboData[4] = (float)volumeMin[0];
  ssboData[5] = (float)volumeMin[1];
  ssboData[6] = (float)volumeMin[2];

  for (int i = 0; i < num; ++i)
  {
    const auto& tool = tools[i];
    int base = 12 + i * 16;

    ssboData[base + 0] = (float)tool.type;
    ssboData[base + 1] = 
      (tool.type == ToolParams::PLANE) ? (float)tool.PlaneOffset :
      (tool.type == ToolParams::CYLINDER) ? (float)tool.CylinderRadius :
      (float)tool.SphereRadius;
    ssboData[base + 2] = (tool.type == ToolParams::CYLINDER) ? (float)tool.CylinderHeight : 0.0f;
    ssboData[base + 3] = 0.0f;

    ssboData[base + 4]  = (float)tool.Transform->GetElement(0, 0);
    ssboData[base + 5]  = (float)tool.Transform->GetElement(0, 1);
    ssboData[base + 6]  = (float)tool.Transform->GetElement(0, 2);
    ssboData[base + 7]  = (float)tool.Transform->GetElement(0, 3);

    ssboData[base + 8]  = (float)tool.Transform->GetElement(1, 0);
    ssboData[base + 9]  = (float)tool.Transform->GetElement(1, 1);
    ssboData[base + 10] = (float)tool.Transform->GetElement(1, 2);
    ssboData[base + 11] = (float)tool.Transform->GetElement(1, 3);

    ssboData[base + 12] = (float)tool.Transform->GetElement(2, 0);
    ssboData[base + 13] = (float)tool.Transform->GetElement(2, 1);
    ssboData[base + 14] = (float)tool.Transform->GetElement(2, 2);
    ssboData[base + 15] = (float)tool.Transform->GetElement(2, 3);
  }

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ToolSSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, ssboData.size() * sizeof(float), ssboData.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GrindingComputeUpdater::UpdateRemovalVolume(
  vtkTextureObject* volume,
  const std::vector<ToolParams>& tools,
  const double volumeMin[3],
  const double volumeMax[3])
{
  if (!volume || !this->ComputeProgram || tools.empty()) return;

  int res[3];
  volume->GetDimensions(res[0], res[1], res[2]);
  if (res[0] <= 0 || res[1] <= 0 || res[2] <= 0) return;

  this->UploadToolData(tools, volumeMin, volumeMax);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ToolSSBO);
  float* mapped = (float*)glMapBufferRange(
    GL_SHADER_STORAGE_BUFFER, 16 + 12, 12,
    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT
  );
  if (mapped)
  {
    mapped[0] = (float)((volumeMax[0] - volumeMin[0]) / res[0]);
    mapped[1] = (float)((volumeMax[1] - volumeMin[1]) / res[1]);
    mapped[2] = (float)((volumeMax[2] - volumeMin[2]) / res[2]);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  }
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  glUseProgram(this->ComputeProgram);
  glBindImageTexture(0, volume->GetHandle(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ToolSSBO);

  glDispatchCompute(
    (res[0] + 7) / 8,
    (res[1] + 7) / 8,
    (res[2] + 7) / 8
  );
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}