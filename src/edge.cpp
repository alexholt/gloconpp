#include "edge.h"

#include <QDebug>

Edge::Edge() {
}

Edge::Edge(const Edge& other) : QObject() {
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

QVector3D Edge::midpoint() const {
  return (m_firstPoint - m_secondPoint) / 2.0f + m_firstPoint;
}

QDebug operator<<(QDebug debug, const Edge& edge) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "Edge: (" << edge.firstPoint() << ", " << edge.secondPoint() << ')';
    return debug;
}
