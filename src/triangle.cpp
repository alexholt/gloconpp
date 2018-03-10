#include <algorithm>
#include <cmath>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <utility>

#include "circle.h"
#include "glocon.h"
#include "triangle.h"

typedef std::pair<Triangle, Triangle> PairOfTriangles;

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

Triangle::Triangle(const QVector2D& top, const QVector2D& left, const QVector2D& bottom) :
  Triangle{
    QVector3D{top.x(), top.y(), 0.0f},
    QVector3D{left.x(), left.y(), 0.0f},
    QVector3D{bottom.x(), bottom.y(), 0.0f},
  } {
}

Triangle::Triangle(const Triangle& other) {
  m_top = other.top();
  m_left = other.left();
  m_bottom = other.bottom();
}

void Triangle::setPoints(const QVector3D& top, const QVector3D& left, const QVector3D& bottom) {
  m_top = top;
  m_left = left;
  m_bottom = bottom;
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
  QList<QVector3D> points;
  points << m_top << m_left << m_bottom;

  // Make sure the points are actually in order or the isValid algorithm won't work right
  std::sort(points.begin(), points.end(), [](const QVector3D& first, const QVector3D& second) -> bool {
    if (qFuzzyCompare(second.y(), first.y()))
      return first.x() < second.x();
    return first.y() < second.y();
  });

  // len(ab) + len(bc) == len(ac) iff they are colinear
  auto a = points[0];
  auto b = points[1];
  auto c = points[2];

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

// See the method in Territory also
bool Triangle::isClockwise() {
  QList<QVector3D> vertices;
  vertices << m_top << m_bottom << m_left;

  float area = 0.0f;
  for (int i = 0; i < vertices.length(); i++) {
    auto first = vertices[i];
    auto second = vertices[(i + 1) % vertices.length()];
    area += first.x() * second.y() - second.x() * first.y();
  }

  return (area / 2) < 0;
}

bool Triangle::isAcute() {
  const auto HALF_PI = M_PI / 2.0 ;

  using namespace glocon;
  return (
    angleBetween(m_top - m_bottom, m_left - m_bottom) < HALF_PI &&
    angleBetween(m_top - m_left, m_bottom - m_left) < HALF_PI &&
    angleBetween(m_left - m_top, m_bottom - m_top) < HALF_PI
  );
}

QVector3D Triangle::normal() {
  auto norman = QVector3D::crossProduct(bottom() - top(), left() - top());
  norman.normalize();
  return norman;
}

std::pair<Triangle, Triangle> Triangle::split() {
  auto first = glocon::angleBetween(m_top - m_bottom, m_left - m_bottom);
  auto second = glocon::angleBetween(m_top - m_left, m_bottom - m_left);
  auto third = glocon::angleBetween(m_left - m_top, m_bottom - m_top);

  std::vector<float> angles;
  angles.push_back(first);
  angles.push_back(second);
  angles.push_back(third);

  std::sort(angles.begin(), angles.end());
  auto max = angles.back();

  Triangle firstChild;
  Triangle secondChild;

  if (max == first) {
    auto midpoint = glocon::midpoint(m_left, m_top);
    firstChild = Triangle(m_top, midpoint, m_bottom);
    secondChild = Triangle(m_left, midpoint, m_bottom);

  } else if (max == second) {
    auto midpoint = glocon::midpoint(m_bottom, m_top);
    firstChild = Triangle(m_top, midpoint, m_left);
    secondChild = Triangle(m_bottom, midpoint, m_left);

  } else if (max == third) {
    auto midpoint = glocon::midpoint(m_left, m_bottom);
    firstChild = Triangle(m_bottom, midpoint, m_top);
    secondChild = Triangle(m_left, midpoint, m_top);
  }

  return std::make_pair(firstChild, secondChild);
}

float Triangle::topLeftAngle() {
  return glocon::angleBetween(m_top - m_bottom, m_left - m_bottom);
}

float Triangle::topBottomAngle() {
  return glocon::angleBetween(m_top - m_left, m_bottom - m_left);
}

float Triangle::leftBottomAngle() {
  return glocon::angleBetween(m_left - m_top, m_bottom - m_top);
}

QDebug operator<<(QDebug debug, const Triangle& tri) {
  QDebugStateSaver saver(debug);
  debug.nospace() << '[' << tri.top() << ", " << tri.left() << ", " << tri.bottom() << ']';

  return debug;
}
