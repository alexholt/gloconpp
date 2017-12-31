#ifndef TERRITORY_H
#define TERRITORY_H

#include <cmath>
#include <QList>
#include <QObject>
#include <QRect>
#include <QVector2D>

#include "pathstate.h"

class Territory : public QObject {
  Q_OBJECT

public:
  Territory(const QString);

private:
  QString m_path;
  QVector2D m_lastPoint;
  QList<QVector2D> m_pointArray;
  PathState m_pathState;
  double m_currentVal = 0;
  QRect m_boundingBox;
  void processToken(QString);
  void stateChange(QString);
  QList<QVector2D> getPointArray();
  QRect getBoundingBox();

signals:

public slots:
};

#endif // TERRITORY_H
