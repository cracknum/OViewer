#ifndef VOLUME_DATA_H
#define VOLUME_DATA_H
#include "DataType.h"
#include <glm/glm.hpp>

struct Volume
{
  glm::vec3 m_Origin;
  glm::vec3 m_Spacing;
  glm::ivec3 m_Dimensions;
  void* m_Data;
  DataType m_DataType;
  glm::mat4 m_IndexToWorld;
  glm::mat4 m_WorldToIndex;

  Volume(const glm::vec3& origin, const glm::vec3& spacing, const glm::ivec3& dimensions,
    const DataType& dataType, void* data = nullptr, const glm::mat4& indexToWorld = glm::mat4(1.0f))
    : m_Origin(origin)
    , m_Spacing(spacing)
    , m_Dimensions(dimensions)
    , m_DataType(dataType)
    , m_Data(data)
    , m_IndexToWorld(indexToWorld)
    , m_WorldToIndex(glm::inverse(indexToWorld))
  {
  }
};
#endif // VOLUME_DATA_H