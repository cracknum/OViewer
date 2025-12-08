#include "ShaderProgram.h"

#include <QFile>
#include <QMatrix4x4>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <qvector2d.h>
#include <spdlog/spdlog.h>
#include <sstream>

struct ShaderProgram::Impl
{
  GLuint m_Program;
  std::vector<GLuint> m_Shaders;

  Impl()
    : m_Program(0)
  {
  }
};

void ShaderProgram::deleteShaders() const
{
  for (const auto& shaderId : m_Impl->m_Shaders)
  {
    glDeleteShader(shaderId);
  }

  m_Impl->m_Shaders.clear();
}
void ShaderProgram::deleteProgram() const
{
  deleteShaders();
  glDeleteProgram(m_Impl->m_Program);
  m_Impl->m_Program = 0;
}
ShaderProgram::ShaderProgram(const ShaderPathMap& shaderSource)
{
  m_Impl = std::make_unique<Impl>();
  m_Impl->m_Program = glCreateProgram();
  std::ostringstream oss;

  for (const auto& pair : shaderSource)
  {
    oss.str("");
    try
    {
      std::string source = loadShaderSource(pair.second);
      GLuint shader = glCreateShader(pair.first);
      const auto sourceStr = source.c_str();
      GLint sourceLength = source.length();
      glShaderSource(shader, 1, &sourceStr, &sourceLength);
      glCompileShader(shader);
      GLint compileStatus = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
      if (compileStatus != GL_TRUE)
      {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1)
        {
          std::vector<GLchar> log;
          glGetShaderInfoLog(shader, logLength, nullptr, log.data());
          oss << "compile error: " << log.data();
        }
        else
        {
          oss << "unknown shader compile error" << pair.second;
        }
        SPDLOG_ERROR(oss.str());
        throw std::runtime_error(oss.str());
      }

      oss << "load shader: " << pair.second;
      glAttachShader(m_Impl->m_Program, shader);
      m_Impl->m_Shaders.push_back(shader);
    }
    catch (std::exception& e)
    {
      oss << e.what() << std::endl;
      deleteProgram();
    }

    SPDLOG_INFO(oss.str());
  }

  glLinkProgram(m_Impl->m_Program);
  GLint linkStatus;
  glGetProgramiv(m_Impl->m_Program, GL_LINK_STATUS, &linkStatus);
  if (linkStatus == GL_FALSE)
  {
    GLsizei logLength;
    glGetProgramiv(m_Impl->m_Program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1)
    {
      std::vector<GLchar> log(logLength);
      glGetProgramInfoLog(m_Impl->m_Program, logLength, nullptr, log.data());
      deleteProgram();
      std::ostringstream logOss;
      logOss << "program link error: " << log.data();
      spdlog::error(logOss.str());
      throw std::runtime_error(logOss.str());
    }

    throw std::runtime_error("error unknown");
  }

  deleteShaders();
}
ShaderProgram::~ShaderProgram() {}
void ShaderProgram::use() const
{
  glUseProgram(m_Impl->m_Program);
}
void ShaderProgram::unuse() const
{
  glUseProgram(0);
}
bool ShaderProgram::setMat4(const glm::mat4& mat4, const char* name) const
{
  const GLint location = glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  glUniformMatrix4fv(location, 1, false, glm::value_ptr(mat4));
  return true;
}
bool ShaderProgram::setInt1(int v, const char* name) const
{
  const GLint location = glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  glUniform1i(location, v);
  return true;
}
bool ShaderProgram::setFloat1(float v, const char* name) const
{
  const GLint location = glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  glUniform1f(location, v);
  return true;
}
bool ShaderProgram::setFloat3(float a, float b, float c, const char* name) const
{
  const GLint location = glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  glUniform3f(location, a, b, c);

  return true;
}
bool ShaderProgram::setVec3(const glm::vec3& vec3, const char* name) const
{
  const GLint location = glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  glUniform3f(location, vec3.x, vec3.y, vec3.z);

  return true;
}

bool ShaderProgram::setVec2(const glm::vec2& vec2, const char* name) const
{
  const GLint location = glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  glUniform2f(location, vec2.x, vec2.y);

  return true;
}
bool ShaderProgram::setVec4(const glm::vec4& vec4, const char* name) const
{
  const GLint location = glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);

  return true;
}
std::string ShaderProgram::loadShaderSource(const char* path)
{
  QFile file(path);
  if (!file.exists())
  {
    std::ostringstream oss;
    oss << "file not exists: " << path;
    throw std::runtime_error(oss.str());
  }

  if (file.open(QIODevice::ReadOnly))
  {
    const QByteArray bytes = file.readAll();
    return bytes.toStdString();
  }

  std::ostringstream oss;
  oss << "file read error: " << path;
  throw std::runtime_error(oss.str());
}
