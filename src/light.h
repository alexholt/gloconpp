#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

#include "model.h"

class Light : public Model {
  Q_OBJECT

public:
  Light(QVector3D);
  virtual ~Light();
  Light(const Light&);
  void setIntensity(float);
  QVector3D color();
  Light& operator=(Light);

private:
  QVector3D m_originalColor;
  QVector3D m_color;
};

#endif // LIGHT_H
