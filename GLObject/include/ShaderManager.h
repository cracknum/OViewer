#ifndef SHADER_MANAGER_OPENGL_H
#define SHADER_MANAGER_OPENGL_H
#include "ShaderProgram.h"
#include <memory>
#include "GLObjectExport.h"

class QOpenGLFunctions_4_4_Core;
enum class PrimitiveType;

class GLOBJECT_API ShaderManager final
{
public:
  using Functions = QOpenGLFunctions_4_4_Core;
  explicit ShaderManager(Functions* functions);
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