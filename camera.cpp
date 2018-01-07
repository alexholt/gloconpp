#include "camera.h"

#define FOV 60
#define FOV_RAD FOV * 2.0 * M_PI / 360.0
#define NEAR 0.01
#define FAR 20000
#define FULLSIZE 2500

using namespace  std;

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

// Fullsize here refers to the z value for the camera that gives a 1 to 1 correspondence
// with the world space coords, i.e., the width of the map == the width of the screen
// The formula for determining the fullsize is derived from the perspective matrix multiplied by the
// vector { width, 0, z, 1 } and solving for z
// -z * aspect * tan(fov/2) == 1
// z = 1 / (aspect * tan(fov/2))
double Camera::fullsize() {
  return -m_width / (m_aspectRatio * tan(FOV_RAD / 2));
}

double Camera::scaleToZ(double scale) {
  return fullsize() * scale * 0.5;
}
