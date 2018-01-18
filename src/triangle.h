#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>

#include "edge.h"

class Circle;

class Triangle {

public:
  Triangle();
  Triangle(const QVector3D&, const QVector3D&, const QVector3D&);
  Triangle(const Triangle&);
  Circle circumCircle();

  const QVector3D& top() const;
  const QVector3D& bottom() const;
  const QVector3D& left() const;

  Edge topEdge() const;
  Edge leftEdge() const;
  Edge bottomEdge() const;

  bool operator ==(const Triangle &other) const;
  bool operator !=(const Triangle &other) const;

  bool sharesEdge(const Edge&) const;
  bool sharesVertex(const Triangle&) const;

private:
  QVector3D m_top;
  QVector3D m_left;
  QVector3D m_bottom;
};

#endif // TRIANGLE_H