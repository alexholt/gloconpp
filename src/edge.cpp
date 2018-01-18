#include "edge.h"

Edge::Edge() {
}

Edge::Edge(const Edge& other) {
  m_firstPoint = other.firstPoint();
  m_secondPoint = other.secondPoint();
}

Edge::Edge(const QVector3D& firstPoint, const QVector3D& secondPoint) {
  m_firstPoint = firstPoint;
  m_secondPoint = secondPoint;
}

const QVector3D& Edge::firstPoint() const {
  return m_firstPoint;
}

const QVector3D& Edge::secondPoint() const {
  return m_secondPoint;
}


bool Edge::operator ==(const Edge& other) const {
  return (qFuzzyCompare(other.firstPoint(), firstPoint()) &&
    qFuzzyCompare(other.secondPoint(), secondPoint())) ||
    (qFuzzyCompare(other.secondPoint(), firstPoint()) &&
    qFuzzyCompare(other.firstPoint(), secondPoint()));
}

Edge Edge::operator =(const Edge& other) const {
  return Edge(other);
}
