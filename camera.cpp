#include "camera.h"

#define FOV 60
#define NEAR 0.01
#define FAR 20000
#define FULLSIZE 2500

Camera::Camera() {
  updateMatrix();
}

Camera::Camera(Camera& other) {
  QVector3D vec = other.position();
  m_position = vec;
  setAspectRatio(other.m_width, other.m_height);
}

QMatrix4x4* Camera::matrix() {
  return &m_matrix;
}

QVector3D& Camera::position() {
  return m_position;
}

void Camera::setAspectRatio(double width, double height) {
  m_width = width;
  m_height = height;
  m_aspectRatio = width / height;
  m_screenVec.setX(width);
  m_screenVec.setY(height);
  m_perspective.setToIdentity();
  m_perspective.perspective(FOV, m_aspectRatio, NEAR, FAR);
}

void Camera::updateMatrix() {
  m_matrix.setToIdentity();
  m_matrix.perspective(FOV, m_aspectRatio, NEAR, FAR);
  m_matrix.translate(m_position);
}

void Camera::translate(float x, float y, float z) {
  m_position += QVector3D{x, y, z};
  updateMatrix();
}

void Camera::moveTo(double x, double y, double z) {
  //auto cameraDistance = -m_position.z();
  //QVector3D probe1{0, 0, -cameraDistance};
  //QVector3D probe2{1, 0, -cameraDistance};

  //auto mappedProbe1 = m_perspective * probe1;
  //auto mappedProbe2 = m_perspective * probe2;

  //auto screenSpaceProbe1 = (mappedProbe1 * 0.5 + QVector3D{0.5, 0.5, 0}) * m_screenVec;
  //auto screenSpaceProbe2 = (mappedProbe2 * 0.5 + QVector3D{0.5, 0.5, 0}) * m_screenVec;
  //auto screenSpaceDelta = screenSpaceProbe2.x() - screenSpaceProbe1.x();
  //auto ratio = 1 / screenSpaceDelta;

  //x *= ratio;
  //y *= ratio;
  m_position = QVector3D{
    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)
  };
  updateMatrix();
}

double Camera::scaleToZ(double scale) {
  return -FULLSIZE * scale;
}
