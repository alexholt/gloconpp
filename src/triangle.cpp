#include <QDebug>

#include "circle.h"
#include "triangle.h"

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

Triangle::Triangle(const Triangle &other) {
  m_top = other.top();
  m_left = other.left();
  m_bottom = other.bottom();
}

Circle Triangle::circumCircle() {
  return Circle(QVector3D{1.0f, 1.0f, 1.0f}, 1.0f);
}

const QVector3D& Triangle::top() const {
  return m_top;
}

const QVector3D& Triangle::left() const {
  return m_left;
}

const QVector3D& Triangle::bottom() const {
  return m_bottom;
}

bool Triangle::operator ==(const Triangle& other) const {
  QList<QVector3D> vertices;
  vertices << m_top << m_left << m_bottom;

  return vertices.contains(other.top()) &&
    vertices.contains(other.left()) &&
    vertices.contains(other.bottom());
}

bool Triangle::operator !=(const Triangle& other) const {
  return !(*this == other);
}

Edge Triangle::topEdge() const {
  return Edge(m_top, m_left);
}

Edge Triangle::leftEdge() const {
  return Edge(m_left, m_bottom);
}

Edge Triangle::bottomEdge() const {
  return Edge(m_bottom, m_top);
}

bool Triangle::sharesEdge(const Edge& other) const {
  return topEdge() == other || leftEdge() == other || bottomEdge() == other;
}

bool Triangle::sharesVertex(const Triangle& other) const {
  QList<QVector3D> vertices;
  vertices << m_top << m_bottom << m_left;

  return vertices.contains(other.m_top) ||
    vertices.contains(other.m_left) ||
    vertices.contains(other.m_bottom);
}
