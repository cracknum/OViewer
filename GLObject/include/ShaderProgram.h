#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <memory>
#include <unordered_map>
#include <string>
#include <QOpenGLFunctions_4_4_Core>
#include "GLObjectExport.h"

enum class ShaderType;
class QMatrix4x4;
class QVector3D;
class QVector4D;

class GLOBJECT_API ShaderProgram final
{
public:
  using Function = QOpenGLFunctions_4_4_Core;
  using ShaderPathMap = std::unordered_map<GLenum, const char*>;
  void deleteShaders(Function* function) const;
  void deleteProgram(Function* function) const;
  explicit ShaderProgram(Function* function, const ShaderPathMap& shaderSource);
  ~ShaderProgram();

  void bind() const;
  void unbind() const;

  // helper function for set variables
  bool setMat4(const QMatrix4x4& mat4, const char* name) const;

  bool setInt1(int v, const char* name) const;
  bool setFloat1(float v, const char* name) const;
  bool setFloat3(float a, float b, float c, const char* name) const;

  bool setVec3(const QVector3D& vec3, const char* name) const;
  bool setVec4(const QVector4D& vec4, const char* name) const;

private:
  [[nodiscard]] static std::string loadShaderSource(const char* path);
private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // SHADER_PROGRAM_H
