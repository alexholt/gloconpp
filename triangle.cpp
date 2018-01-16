#include "triangle.h"

#include "circle.h"

Triangle::Triangle() {
  m_top = QVector3D{0.0f, 1.0f, 0.0f};
  m_left = QVector3D{-1.0f, 0.0f, 0.0f};
  m_bottom = QVector3D{1.0f, 0.0f, 0.0f};
}

Triangle::Triangle(const QVector3D& top, const QVector3D& left, const QVector3D& bottom) {
  m_top = top;
  m_left = left;
  m_bottom = bottom;
}

Circle Triangle::circumCircle() {
  return Circle(QVector3D{1.0f, 1.0f, 1.0f}, 1.0f);
}
