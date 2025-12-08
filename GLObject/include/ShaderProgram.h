#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include "GLObjectExport.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

enum class ShaderType;

class GLOBJECT_API ShaderProgram final
{
public:
  using ShaderPathMap = std::unordered_map<GLenum, const char*>;
  void deleteShaders() const;
  void deleteProgram() const;
  explicit ShaderProgram(const ShaderPathMap& shaderSource);
  ~ShaderProgram();

  void use() const;
  void unuse() const;

  // helper function for set variables
  bool setMat4(const glm::mat4& mat4, const char* name) const;

  bool setInt1(int v, const char* name) const;
  bool setFloat1(float v, const char* name) const;
  bool setFloat3(float a, float b, float c, const char* name) const;

  bool setVec2(const glm::vec2& vec2, const char* name) const;
  bool setVec3(const glm::vec3& vec3, const char* name) const;
  bool setVec4(const glm::vec4& vec4, const char* name) const;

private:
  [[nodiscard]] static std::string loadShaderSource(const char* path);

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // SHADER_PROGRAM_H
