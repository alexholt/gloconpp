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
