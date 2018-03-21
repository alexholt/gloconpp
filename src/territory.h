#ifndef TERRITORY_H
#define TERRITORY_H

#include <cmath>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QVector2D>

#include "model.h"
#include "pathstate.h"
#include "rect.h"
#include "triangle.h"

class Territory : public Model {

  Q_OBJECT

public:
  Territory(const QString&);
  Territory(const QString&, const QString&);
  Territory(const QString&, const QString&, bool);
  ~Territory();
  Rect getBoundingBox();
  Triangle getSuperTriangle();
  QVector2D* getCentroid();
  void buildMesh();
  QList<Triangle> getMesh();
  bool fuzzyVertexCompare(const QVector3D&, const QVector3D&) const;
  bool isClockwise();
  QString getName();
  void subdivide();
  void buildVerticesFromPointList(QList<QVector3D>);
  void intersection(Territory&);
  QList<QVector3D> pointList();

private:
  QString m_path;
  QVector2D* m_lastPoint = new QVector2D;
  QList<QVector2D*> m_pointArray;
  QList<Triangle> m_mesh;
  QVector2D* m_centroid = nullptr;
  PathState m_pathState;
  float m_currentVal = 0;
  Rect m_boundingBox;
  void processToken(QString);
  void stateChange(QString);
  QList<QVector2D*>& getPointArray();
  QString m_name = "?";
  void subdivideSingle(Triangle tri, QList<Triangle>& subdivided);
};

#endif // TERRITORY_H
