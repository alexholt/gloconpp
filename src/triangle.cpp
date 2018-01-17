#include "triangle.h"

#include "circle.h"
#include <QDebug>

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

Triangle::Triangle(Triangle &other) {
  qDebug() << "I am being copied";
  m_top = other.top();
  m_left = other.left();
  m_bottom = other.bottom();
}

Circle Triangle::circumCircle() {
  return Circle(QVector3D{1.0f, 1.0f, 1.0f}, 1.0f);
}

QVector3D& Triangle::top() {
  return m_top;
}

QVector3D& Triangle::left() {
  return m_left;
}

QVector3D& Triangle::bottom() {
  return m_bottom;
}

bool Triangle::operator ==(Triangle& other) {
  return m_top == other.top() && m_left == other.left() && m_bottom == other.bottom();
}
