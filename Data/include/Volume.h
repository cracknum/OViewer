#ifndef VOLUME_DATA_H
#define VOLUME_DATA_H
#include <glm/glm.hpp>
#include <memory>

template <typename ComponentType>
struct TypeTraits
{
};

template <>
struct TypeTraits<float>
{
  using ValueType = float;
};
template <>
struct TypeTraits<int>
{
  using ValueType = int;
};
template <>
struct TypeTraits<short>
{
  using ValueType = short;
};
template <>
struct TypeTraits<char>
{
  using ValueType = char;
};
template <>
struct TypeTraits<unsigned char>
{
  using ValueType = unsigned char;
};

template <typename ComponentType = float>
struct Volume
{
  using DataType = typename TypeTraits<ComponentType>::ValueType;
  using DataTypePointer = DataType*;
  glm::vec3 m_Origin;
  glm::vec3 m_Spacing;
  glm::ivec3 m_Dimensions;
  DataTypePointer m_Data;
  glm::mat4 m_IndexToWorld;
  glm::mat4 m_WorldToIndex;

  Volume(const glm::vec3& origin, const glm::vec3& spacing, glm::ivec3& dimensions,
    DataTypePointer data = nullptr, const glm::mat4& indexToWorld = glm::mat4(1.0f))
    : m_Origin(origin)
    , m_Spacing(spacing)
    , m_Dimensions(dimensions)
    , m_Data(data)
    , m_IndexToWorld(indexToWorld)
    , m_WorldToIndex(glm::inverse(indexToWorld))
  {
  }
};
#endif // VOLUME_DATA_H