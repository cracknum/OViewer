#include "ShaderProgram.h"

#include <QFile>
#include <iostream>
#include <sstream>
#include <QMatrix4x4>

struct ShaderProgram::Impl
{
  GLuint m_Program;
  std::vector<GLuint> m_Shaders;
  Function* m_Function;

  Impl()
    : m_Program(0)
    , m_Function(nullptr)
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
ShaderProgram::ShaderProgram(Function* function, const ShaderSourceMap& shaderSource)
{
  m_Impl = std::make_unique<Impl>();
  m_Impl->m_Program = function->glCreateProgram();
  for (const auto& pair : shaderSource)
  {
    try
    {
      std::string source = loadShaderSource(pair.second);
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      deleteProgram(function);
    }

    GLuint shader = function->glCreateShader(pair.first);
    m_Impl->m_Shaders.push_back(shader);

    function->glCompileShader(shader);
    function->glAttachShader(m_Impl->m_Program, shader);
  }

  function->glLinkProgram(m_Impl->m_Program);
  GLint linkStatus;
  function->glGetProgramiv(m_Impl->m_Program, GL_COMPILE_STATUS, &linkStatus);
  if (linkStatus == GL_FALSE)
  {
    GLsizei logLength;
    function->glGetProgramiv(m_Impl->m_Program, GL_INFO_LOG_LENGTH, &logLength);
    const auto log = new GLchar[logLength]{};
    function->glGetProgramInfoLog(m_Impl->m_Program, logLength, nullptr, log);
    deleteProgram(function);
    std::ostringstream oss;
    oss << "program link error: " << log;
    throw std::runtime_error(oss.str());
  }
  deleteShaders(function);
}
ShaderProgram::~ShaderProgram() {}
void ShaderProgram::bind() const
{
  m_Impl->m_Function->glUseProgram(m_Impl->m_Program);
}
void ShaderProgram::unbind() const
{
  m_Impl->m_Function->glUseProgram(0);
}
void ShaderProgram::setMat4(const QMatrix4x4& mat4, const char* name) const
{
  const GLint location =  m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  m_Impl->m_Function->glUniformMatrix4fv(location, 1, false, mat4.data());
}
void ShaderProgram::setInt1(int v, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  m_Impl->m_Function->glUniform1i(location, v);
}
void ShaderProgram::setFloat1(float v, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  m_Impl->m_Function->glUniform1f(location, v);
}
void ShaderProgram::setFloat3(float a, float b, float c, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  m_Impl->m_Function->glUniform3f(location, a, b, c);
}
void ShaderProgram::setVec3(const QVector3D& vec3, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  m_Impl->m_Function->glUniform3f(location, vec3.x(), vec3.y(), vec3.z());
}
void ShaderProgram::setVec4(const QVector4D& vec4, const char* name) const
{
  const GLint location = m_Impl->m_Function->glGetUniformLocation(m_Impl->m_Program, name);
  m_Impl->m_Function->glUniform4f(location, vec4.x(), vec4.y(), vec4.z(), vec4.w());
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