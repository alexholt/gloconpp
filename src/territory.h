#ifndef TERRITORY_H
#define TERRITORY_H

#include <cmath>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QVector2D>

#include "pathstate.h"
#include "rect.h"
#include "triangle.h"

class Territory : public QObject {
  Q_OBJECT

public:
  Territory(const QString);
  ~Territory();
  Rect getBoundingBox();
  Triangle getSuperTriangle();
  QVector2D* getCentroid();
  QList<QVector3D>& getMesh();

private:
  QString m_path;
  QVector2D* m_lastPoint = new QVector2D;
  QList<QVector2D*> m_pointArray;
  QList<QVector3D> m_mesh;
  QVector2D* m_centroid = nullptr;
  PathState m_pathState;
  float m_currentVal = 0;
  Rect m_boundingBox;
  void processToken(QString);
  void stateChange(QString);
  QList<QVector2D*>& getPointArray();
};

#endif // TERRITORY_H
