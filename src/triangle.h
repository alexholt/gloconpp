#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>

class Circle;

class Triangle {

public:
  Triangle();
  Triangle(const QVector3D&, const QVector3D&, const QVector3D&);
  Triangle(Triangle&);
  Circle circumCircle();

  QVector3D& top();
  QVector3D& bottom();
  QVector3D& left();

  bool operator ==(Triangle &other);

private:
  QVector3D m_top;
  QVector3D m_left;
  QVector3D m_bottom;
};

#endif // TRIANGLE_H
