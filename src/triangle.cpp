#include <algorithm>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>

#include "circle.h"
#include "triangle.h"

Triangle::Triangle() {
  m_top = QVector3D{0.0f, 1.0f, 0.0f};
  m_left = QVector3D{-1.0f, 0.0f, 0.0f};
  m_bottom = QVector3D{1.0f, 0.0f, 0.0f};
}

Triangle::Triangle(const QVector3D& top, const QVector3D& left, const QVector3D& bottom) {
  QList<QVector3D> points;
  points << top << left << bottom;

  // Make sure the points are actually in order or the isValid algorithm won't work right
  std::sort(points.begin(), points.end(), [](const QVector3D& first, const QVector3D& second) -> bool {
    if (qFuzzyCompare(second.y(), first.y()))
      return first.x() > second.x();
    return first.y() > second.y();
  });

  m_top = points[0];

  points.removeAll(points[0]);

  std::sort(points.begin(), points.end(), [](const QVector3D& first, const QVector3D& second) -> bool {
    if (qFuzzyCompare(second.x(), first.x()))
      return first.y() > second.y();
    return first.x() > second.x();
  });

  m_left = points[0];
  m_bottom = points[1];
}

Triangle::Triangle(const QVector2D& top, const QVector2D& left, const QVector2D& bottom) :
  Triangle{
    QVector3D{top.x(), top.y(), 0.0f},
    QVector3D{left.x(), left.y(), 0.0f},
    QVector3D{bottom.x(), bottom.y(), 0.0f},
  } {
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

bool Triangle::isValid() {
  // len(ab) + len(bc) == len(ac) iff they are colinear
  auto a = m_top;
  auto b = m_left;
  auto c = m_bottom;

  return !qFuzzyCompare((b - a).length() + (c - b).length(), (c - a).length());
}

bool Triangle::contains(const QVector3D& v) const {
  // http://mathworld.wolfram.com/TriangleInterior.html
  auto v0 = m_left;
  auto v1 = m_top - m_left;
  auto v2 = m_bottom - m_left;
  auto det = [](const QVector3D& first, const QVector3D& second) {
    return first.x() * second.y() - first.y() * second.x();
  };
  auto a = (det(v, v2) - det(v0, v2)) / det(v1, v2);
  auto b = -(det(v, v1) - det(v0, v1)) / det(v1, v2);
  auto doesContain = a > 0 && b > 0 && (a + b) < 1;
  return doesContain;
}

QDebug operator<<(QDebug debug, const Triangle& tri) {
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << tri.top() << ", " << tri.left() << tri.bottom() << ')';

    return debug;
}
