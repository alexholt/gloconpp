#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

class Camera : public QObject {
  Q_OBJECT

public:
  Camera();
  QMatrix4x4 matrix();
  void setAspectRatio(qreal);

private:
  QMatrix4x4 m_matrix;
  qreal m_aspectRatio = 2;
  QVector3D m_position{-500, 300, -500};
  void updateMatrix();

signals:

public slots:
};

#endif // CAMERA_H
