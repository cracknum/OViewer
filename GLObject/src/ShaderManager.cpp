#include "ShaderManager.h"
#include "PrimitiveType.h"
#include <QOpenGLFunctions_4_4_Core>

struct ShaderManager::Impl
{
  std::unordered_map<PrimitiveType, std::shared_ptr<ShaderProgram>> m_ShaderCache;
  Functions* m_Functions;
  Impl(Functions* functions)
    : m_Functions(functions)
  {
  }
};

ShaderManager::ShaderManager(Functions* functions)
{
  m_Impl = std::make_unique<Impl>(functions);
}

ShaderManager::~ShaderManager() = default;

ShaderManager::ShaderPointer ShaderManager::registerShader(const PrimitiveType& primitiveType, const ShaderPathMap& pathMap)
{
  if (m_Impl->m_ShaderCache.find(primitiveType) != m_Impl->m_ShaderCache.end())
  {
    return m_Impl->m_ShaderCache[primitiveType];
  }

  m_Impl->m_ShaderCache[primitiveType] =
    std::make_shared<ShaderProgram>(m_Impl->m_Functions, pathMap);

  return m_Impl->m_ShaderCache[primitiveType];
}

ShaderManager::ShaderPointer ShaderManager::getShader(const PrimitiveType& primitiveType)
{
  if (m_Impl->m_ShaderCache.find(primitiveType) != m_Impl->m_ShaderCache.end())
  {
    return m_Impl->m_ShaderCache[primitiveType];
  }

  return nullptr;
}
