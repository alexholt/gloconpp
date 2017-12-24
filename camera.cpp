#include "camera.h"

Camera::Camera() {
  updateMatrix();
}

QMatrix4x4* Camera::matrix() {
  return &m_matrix;
}

void Camera::setAspectRatio(qreal aspectRatio) {
  m_aspectRatio = aspectRatio;
  updateMatrix();
}

void Camera::updateMatrix() {
  m_matrix.setToIdentity();
  m_matrix.perspective(60, m_aspectRatio, 0.1, 20000);
  //m_matrix.translate(m_position);
}
