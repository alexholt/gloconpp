#include "circle.h"

#include "triangle.h"

Circle::Circle(const QVector3D& center, float radius) {
  m_center = center;
  m_radius = radius;
}

Circle::Circle(const Triangle& circum) {
  Q_UNUSED(circum)
}
