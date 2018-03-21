#include <QVector3D>

#include "light.h"

Light::Light(QVector3D initialColor) {
  m_color = m_originalColor = initialColor;
  //loadFile(":/assets/models/mars.obj", "light");
}

Light::Light(const Light& other):
  Light(other.m_originalColor) {
  m_modelViewMatrix = other.m_modelViewMatrix;
}

Light::~Light() {

}

Light& Light::operator=(Light other) {
  m_originalColor = other.m_originalColor;
  m_modelViewMatrix = other.m_modelViewMatrix;
  return *this;
}

void Light::setIntensity(float intensity) {
  m_color = QVector3D{
    intensity * m_originalColor.x(),
    intensity * m_originalColor.y(),
    intensity * m_originalColor.z(),
  };
}

QVector3D Light::color() {
  return m_color;
}
