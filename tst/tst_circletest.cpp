#include <QtTest>
#include <QVector3D>

#include "circle.h"
#include "triangle.h"

#include "tst_circletest.h"

void CircleTest::initTestCase() {
  m_subject = new Circle(
    Triangle{
      QVector3D{0.5f, 0.0f, 0.0f},
      QVector3D{0.0f, 0.5f, 0.0f},
      QVector3D{0.0f, -0.5f, 0.0f}
    }
  );
}

void CircleTest::constructedFromATriangle() {
  QCOMPARE(m_subject->radius(), 0.5f);

  // The x term is out of the threshold of fuzzy compare unfortunately
  QVERIFY(m_subject->center().x() < 3.0e-8);
  QCOMPARE(m_subject->center().y(), 0.0f);
  QCOMPARE(m_subject->center().z(), 0.0f);
}

void CircleTest::cleanupTestCase() {
  delete m_subject;
}
