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

  QVector3D topLeft = m_matrix * QVector3D{-1000.0f, 500.0f, -0.5f};
  topLeft.setX((topLeft.x() * 0.5 + 0.5) * 800);
  topLeft.setY((topLeft.y() * 0.5 + 0.5) * 800);

  QVector3D bottomRight = m_matrix * QVector3D{1000.0f, -500.0f, -0.5f};
  bottomRight.setX((bottomRight.x() * 0.5 + 0.5) * 800);
  bottomRight.setY((bottomRight.y() * 0.5 + 0.5) * 800);

  qDebug() << "contentWidth" << bottomRight.x() - topLeft.x();
  qDebug() << "contentHeight" << topLeft.y() - bottomRight.y();
}
