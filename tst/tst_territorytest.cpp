#include <QtTest>
#include <QVector3D>

#include "edge.h"
#include "territory.h"
#include "triangle.h"

#include "tst_territorytest.h"

void TerritoryTest::initTestCase() {
  m_subject = new Territory("m 0,0 10,0 0,10 z");
}

void TerritoryTest::cleanupTestCase() {
  delete m_subject;
}

void TerritoryTest::meshBuildingTestCase() {
  QVERIFY(m_subject->getMesh().length() > 0);
  for (auto point : m_subject->getMesh()) {
    qDebug() << point;
  }
}

void TerritoryTest::boundingBoxTestCase() {
  QCOMPARE(m_subject->getBoundingBox().x(), 0.0f);
  QCOMPARE(m_subject->getBoundingBox().x2(), 10.0f);
  QCOMPARE(m_subject->getBoundingBox().y(), 0.0f);
  QCOMPARE(m_subject->getBoundingBox().y2(), 10.0f);
  QCOMPARE(m_subject->getBoundingBox().width(), 10.0f);
  QCOMPARE(m_subject->getBoundingBox().height(), 10.0f);
}

void TerritoryTest::getSuperTriangleTestCase() {
  auto superTriangle = m_subject->getSuperTriangle();
  auto box = m_subject->getBoundingBox();
  QList<QVector3D> triPoints;
  triPoints << superTriangle.bottom() << superTriangle.top() << superTriangle.left();

  QVERIFY(superTriangle.bottom().x() > box.x());
}