#ifndef TST_TERRITORYTEST_H
#define TST_TERRITORYTEST_H

#include <QObject>

class Territory;

class TerritoryTest : public QObject {
  Q_OBJECT

private:
  Territory* m_subject;

private slots:
  void initTestCase();
  void cleanupTestCase();
  void boundingBoxTestCase();
  void meshBuildingTestCase();
  void getSuperTriangleTestCase();
};

#endif // TST_TERRITORYTEST_H
