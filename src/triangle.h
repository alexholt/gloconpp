#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>

#include "edge.h"
#include "glocon.h"

class Circle;

class Triangle {

public:
  Triangle();
  Triangle(const QVector3D&, const QVector3D&, const QVector3D&);
  Triangle(const QVector2D&, const QVector2D&, const QVector2D&);
  Triangle(const Triangle&);
  void setPoints(const QVector3D&, const QVector3D&, const QVector3D&);
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
  bool contains(const QVector3D&) const;

  bool isValid();
  bool isClockwise();
  bool isAcute();

  QVector3D normal();
  std::pair<Triangle, Triangle> split();
  float topLeftAngle();
  float topBottomAngle();
  float leftBottomAngle();
  void flip();

private:
  QVector3D m_top;
  QVector3D m_left;
  QVector3D m_bottom;
};

QDebug operator<<(QDebug, const Triangle&);

#endif // TRIANGLE_H
