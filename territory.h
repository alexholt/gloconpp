#ifndef TERRITORY_H
#define TERRITORY_H

#include <cmath>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QRect>
#include <QVector2D>

#include "pathstate.h"

class Territory : public QObject {
  Q_OBJECT

public:
  Territory(const QString);
  ~Territory();
  QRect getBoundingBox();
  QVector2D* getCentroid();
  QList<QVector3D*>& getMesh();

private:
  QString m_path;
  QVector2D* m_lastPoint = new QVector2D;
  QList<QVector2D*> m_pointArray;
  QList<QVector3D*> m_mesh;
  QVector2D* m_centroid = nullptr;
  PathState m_pathState;
  float m_currentVal = 0;
  QRect m_boundingBox;
  void processToken(QString);
  void stateChange(QString);
  QList<QVector2D*>& getPointArray();
};

#endif // TERRITORY_H
