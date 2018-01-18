#include <cmath>
#include <QVector3D>

#include "circle.h"
#include "triangle.h"

using namespace std;

Circle::Circle(const QVector3D& center, float radius) {
  m_center = center;
  m_radius = radius;
}

Circle::Circle(const Triangle& circum) {
  auto a = circum.left() - circum.top();
  auto b = circum.bottom() - circum.top();
  m_radius = a.length() * b.length() * (a - b).length() / (2 * QVector3D::crossProduct(a, b).length());
  auto leftTerm = (powf(a.length(), 2.0f) * b - powf(b.length(), 2.0f) * a);
  auto rightTerm = QVector3D::crossProduct(a, b);
  leftTerm = QVector3D::crossProduct(leftTerm, rightTerm);
  leftTerm /= 2 * powf(QVector3D::crossProduct(a, b).length(), 2.0f);
  leftTerm += circum.top();
  m_center = leftTerm;
}

float Circle::radius() {
  return m_radius;
}

QVector3D& Circle::center() {
  return m_center;
}

bool Circle::contains(const QVector3D& point) {
  if (!qFuzzyCompare(point.z(), m_center.z()))
    return false;
  return point.distanceToPoint(m_center) <= m_radius;
}
