#ifndef CAMERA_H
#define CAMERA_H

#include <cassert>
#include <cmath>
#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

class Camera : public QObject {
  Q_OBJECT

public:
  Camera();
  QMatrix4x4* matrix();
  void setAspectRatio(qreal);
  void translate(float, float, float);
  QVector3D* position();

private:
  QMatrix4x4 m_matrix;
  qreal m_aspectRatio = 2.0f;
  QVector3D m_position{0.25f, 0.0f, -500.0f};
  void updateMatrix();
};

#endif // CAMERA_H
