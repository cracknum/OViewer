#include "PlaneSource.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace
{
struct Point
{
  glm::vec3 mData;
  bool mValid;
  Point()
    : mData{}
    , mValid(false)
  {
  }

  void setData(double data[3]) { mData = glm::vec3(data[0], data[1], data[2]); }
};
}

struct PlaneSource::Private
{
  Point mOrigin;
  Point mPoint1;
  Point mPoint2;
  Point mNormal;

  std::shared_ptr<Vertices> mVertices;
  glm::mat4 mIndexToWorldMatrix;

  Private()
    : mIndexToWorldMatrix(1.0f)
  {
    mVertices = std::make_shared<Vertices>();
  }
};
PlaneSource::PlaneSource()
{
  mPrivate = std::make_unique<Private>();
}

PlaneSource::~PlaneSource() {}

void PlaneSource::setOrigin(double origin[3])
{
  mPrivate->mOrigin.setData(origin);
}

void PlaneSource::setPoint1(double point[3])
{
  mPrivate->mPoint1.setData(point);
}

void PlaneSource::setPoint2(double point[3])
{
  mPrivate->mPoint2.setData(point);
}

std::shared_ptr<Vertices> PlaneSource::polyData()
{
  return mPrivate->mVertices;
}

glm::mat4 PlaneSource::indexToWorldMatrix()
{
  return mPrivate->mIndexToWorldMatrix;
}

void PlaneSource::updateGeometry()
{
  if (!mPrivate->mOrigin.mValid || !mPrivate->mPoint1.mValid || !mPrivate->mPoint2.mValid)
  {
    return;
  }

  auto right = mPrivate->mPoint1.mData - mPrivate->mOrigin.mData;
  auto top = mPrivate->mPoint2.mData - mPrivate->mOrigin.mData;
  auto normal = glm::normalize(glm::cross(right, top));

  // index to world matrix
  mPrivate->mIndexToWorldMatrix[0] = glm::vec4(right, 0.0f);
  mPrivate->mIndexToWorldMatrix[1] = glm::vec4(top, 0.0f);
  mPrivate->mIndexToWorldMatrix[2] = glm::vec4(normal, 0.0f);
  mPrivate->mIndexToWorldMatrix[3] = glm::vec4(mPrivate->mOrigin.mData, 1.0f);

  glm::length(right);
  // clang-format off
  mPrivate->mVertices->m_Data = new GLfloat[32]{ 
	// vertex   texture   normal
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	glm::length(right), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, glm::length(top), 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    glm::length(right), glm::length(top), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
  };
  // clang-format on
  
  mPrivate->mVertices->m_DataSize = 32;
  mPrivate->mVertices->m_PointAttribute.first = true;
  mPrivate->mVertices->m_PointAttribute.second = 3;
  mPrivate->mVertices->m_Indices = new GLuint[6]{ 0, 1, 3, 1, 2, 3 };
  mPrivate->mVertices->m_IndicesSize = 6;
  mPrivate->mVertices->m_NormalAttribute.first = true;
  mPrivate->mVertices->m_NormalAttribute.second = 3;
  mPrivate->mVertices->m_TextureAttribute.first = true;
  mPrivate->mVertices->m_TextureAttribute.second = 2;
}
