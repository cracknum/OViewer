#include "ShaderProgram.h"

#include <QFile>
#include <iostream>
#include <sstream>
#include <QMatrix4x4>
#include <spdlog/spdlog.h>
#include <qvector2d.h>

struct ShaderProgram::Impl
{
  GLuint m_Program;
  std::vector<GLuint> m_Shaders;
  Function* m_Function;

  Impl(Function* functions)
    : m_Program(0)
    , m_Function(functions)
  {
  }
};

void ShaderProgram::deleteShaders(ShaderProgram::Function* function) const
{
  for (const auto& shaderId : m_Impl->m_Shaders)
  {
    function->glDeleteShader(shaderId);
  }

  m_Impl->m_Shaders.clear();
}
void ShaderProgram::deleteProgram(ShaderProgram::Function* function) const
{
  deleteShaders(function);
  function->glDeleteProgram(m_Impl->m_Program);
  m_Impl->m_Program = 0;
}
ShaderProgram::ShaderProgram(Function* function, const ShaderPathMap& shaderSource)
{
  m_Impl = std::make_unique<Impl>(function);
  m_Impl->m_Program = function->glCreateProgram();
  std::ostringstream oss;

  for (const auto& pair : shaderSource)
  {
    oss.str("");
    try
    {
      std::string source = loadShaderSource(pair.second);
      GLuint shader = function->glCreateShader(pair.first);
      const auto sourceStr = source.c_str();
      GLint sourceLength = source.length();
      function->glShaderSource(shader, 1, &sourceStr, &sourceLength);
      function->glCompileShader(shader);
      GLint compileStatus = 0;
      function->glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
      if (compileStatus != GL_TRUE)
      {
        GLint logLength = 0;
        function->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1)
        {
          std::vector<GLchar> log;
          function->glGetShaderInfoLog(shader, logLength, nullptr, log.data());
          oss << "compile error: " << log.data();
        }
        else
        {
          oss << "unknown shader compile error" << pair.second;
        }
        spdlog::error(oss.str());
        throw std::runtime_error(oss.str());
      }

      oss << "load shader: " << pair.second;
      function->glAttachShader(m_Impl->m_Program, shader);
      m_Impl->m_Shaders.push_back(shader);
    }
    catch (std::exception& e)
    {
      oss << e.what() << std::endl;
      deleteProgram(function);
    }

    spdlog::info(oss.str());

  }

  function->glLinkProgram(m_Impl->m_Program);
  GLint linkStatus;
  function->glGetProgramiv(m_Impl->m_Program, GL_LINK_STATUS, &linkStatus);
  if (linkStatus == GL_FALSE)
  {
        GLsizei logLength;
        function->glGetProgramiv(m_Impl->m_Program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1)
        {
          std::vector<GLchar> log(logLength);
          function->glGetProgramInfoLog(m_Impl->m_Program, logLength, nullptr, log.data());
          deleteProgram(function);
          std::ostringstream oss;
          oss << "program link error: " << log.data();
          spdlog::error(oss.str());
          throw std::runtime_error(oss.str());
        }
        
        throw std::runtime_error("error unknown");
  }

    deleteShaders(function);
}
ShaderProgram::~ShaderProgram() {}
void ShaderProgram::use() const
{
  m_Impl->m_Function->glUseProgram(m_Impl->m_Program);
}
void ShaderProgram::unuse() const
{
  m_Impl->m_Function->glUseProgram(0);
}
bool ShaderProgram::setMat4(const QMatrix4x4& mat4, const char* name) const
{
  const GLint location =  m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  m_Impl->m_Function->glUniformMatrix4fv(location, 1, false, mat4.data());
  return true;
}
bool ShaderProgram::setInt1(int v, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  m_Impl->m_Function->glUniform1i(location, v);
  return true;
}
bool ShaderProgram::setFloat1(float v, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  m_Impl->m_Function->glUniform1f(location, v);
  return true;
}
bool ShaderProgram::setFloat3(float a, float b, float c, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  m_Impl->m_Function->glUniform3f(location, a, b, c);
  
  return true;
}
bool ShaderProgram::setVec3(const QVector3D& vec3, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  m_Impl->m_Function->glUniform3f(location, vec3.x(), vec3.y(), vec3.z());
  
  return true;
}

bool ShaderProgram::setVec2(const QVector2D& vec2, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  m_Impl->m_Function->glUniform2f(location, vec2.x(), vec2.y());

  return true;
}
bool ShaderProgram::setVec4(const QVector4D& vec4, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  if (location == -1)
  {
    return false;
  }
  m_Impl->m_Function->glUniform4f(location, vec4.x(), vec4.y(), vec4.z(), vec4.w());

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
