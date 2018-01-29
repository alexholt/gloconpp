#include <QtTest>

#include "triangle.h"

#include "tst_triangletest.h"

TriangleTest::TriangleTest() {

}

TriangleTest::~TriangleTest() {

}

void TriangleTest::initTestCase() {
  m_subject = new Triangle;
}

void TriangleTest::cleanupTestCase() {
  delete m_subject;
}

void TriangleTest::test_case_copy_constructor() {
  Triangle copy = *m_subject;
  QVERIFY2(*m_subject == copy, "The copy constructor works");
  QList<Triangle> triangles;
  triangles << Triangle();
}

void TriangleTest::testCaseisValid() {
  QVERIFY2(m_subject->isValid(), "The validation check works");
  QVERIFY2(
        !Triangle(
          QVector3D{0.0f, 0.0f, 0.0f},
          QVector3D{0.0f, 10.0f, 0.0f},
          QVector3D{0.0f, -10.0f, 0.0f}
        ).isValid(),
    "The validation check will fail if the three points are colinear"
  );
}

void TriangleTest::testCaseContains() {
  delete m_subject;
  m_subject = new Triangle(
    QVector3D{0.0f, 0.0f, 0.0f},
    QVector3D{0.0f, 1.0f, 0.0f},
    QVector3D{1.0f, 0.0f, 0.0f}
  );
  QVERIFY(m_subject->contains(QVector3D{0.5f, 0.5f, 0.0f}));
  QVERIFY(!m_subject->contains(QVector3D{-0.5f, 0.5f, 0.0f}));
}
