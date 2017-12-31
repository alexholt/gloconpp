#include "camera.h"

Camera::Camera() {
  updateMatrix();
}

QMatrix4x4* Camera::matrix() {
  return &m_matrix;
}

QVector3D& Camera::position() {
  return m_position;
}

void Camera::setAspectRatio(qreal aspectRatio) {
  m_aspectRatio = aspectRatio;
  updateMatrix();
}

void Camera::updateMatrix() {
  m_matrix.setToIdentity();
  m_matrix.perspective(60, m_aspectRatio, 0.1, 20000);
  m_matrix.translate(m_position);
}

void Camera::translate(float x, float y, float z) {
  m_position += QVector3D{x, y, z};
  updateMatrix();
}

void Camera::moveTo(double x, double y, double z) {
  m_position = QVector3D{
    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)
  };
  updateMatrix();
  qDebug() << (m_matrix.inverted() * QVector3D{0, 0, 0});
}
