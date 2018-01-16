#ifndef CIRCLE_H
#define CIRCLE_H

#include <QVector3D>

class Triangle;

class Circle {

public:
  Circle(const QVector3D&, float);
  Circle(const Triangle&);

private:
  float m_radius;
  QVector3D m_center;
};

#endif // CIRCLE_H
