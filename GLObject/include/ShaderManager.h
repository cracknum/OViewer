#ifndef SHADER_MANAGER_OPENGL_H
#define SHADER_MANAGER_OPENGL_H
#include "GLObjectExport.h"
#include "ShaderProgram.h"
#include <memory>

enum class PrimitiveType;

class GLOBJECT_API ShaderManager final
{
public:
  explicit ShaderManager();
  ~ShaderManager();
  using ShaderPathMap = ShaderProgram::ShaderPathMap;
  using ShaderPointer = std::shared_ptr<ShaderProgram>;
  ShaderPointer registerShader(const PrimitiveType& primitiveType, const ShaderPathMap& pathMap);
  ShaderPointer getShader(const PrimitiveType& primitiveType);

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};
#endif