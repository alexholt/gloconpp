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
  Camera(Camera&);
  QMatrix4x4* matrix();
  void setAspectRatio(double, double);
  void translate(float, float, float);
  QVector3D& position();
  void moveTo(double, double, double);
  double scaleToZ(double);

private:
  QMatrix4x4 m_matrix;
  QMatrix4x4 m_perspective;
  double m_aspectRatio = 2.0;
  double m_width = 1.0;
  double m_height = 1.0;
  QVector3D m_position{0.0f, 0.0f, -2500.0f};
  QVector3D m_screenVec{1, 1, 1};
  void updateMatrix();
};

#endif // CAMERA_H
