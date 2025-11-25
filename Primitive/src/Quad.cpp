#include "Quad.h"

Quad::Quad(const glm::vec3& origin, const glm::vec3& u, const glm::vec3& v)
  : m_Origin(origin)
  , m_U(u)
  , m_V(v)
{
}

Quad::~Quad() {}

void Quad::mousePressEvent(QMouseEvent* event) {}

void Quad::mouseReleaseEvent(QMouseEvent* event) {}

void Quad::mouseDoubleClickEvent(QMouseEvent* event) {}

void Quad::mouseMoveEvent(QMouseEvent* event) {}

void Quad::wheelEvent(QWheelEvent* event) {}

void Quad::keyPressEvent(QKeyEvent* event) {}

void Quad::keyReleaseEvent(QKeyEvent* event) {}

void Quad::draw() {}
