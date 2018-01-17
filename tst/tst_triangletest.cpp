#include <QtTest>

#include "triangle.h"

class TriangleTest : public QObject {
  Q_OBJECT

public:
  TriangleTest();
  ~TriangleTest();

private:
  Triangle* m_subject;

private slots:
  void initTestCase();
  void cleanupTestCase();
  void test_case_copy_constructor();

};

TriangleTest::TriangleTest() {

}

TriangleTest::~TriangleTest() {

}

void TriangleTest::initTestCase() {
  qDebug() << "Creating a triangle";
  m_subject = new Triangle;
}

void TriangleTest::cleanupTestCase() {
  delete m_subject;
}

Triangle duplicate(Triangle tri) {
  return tri;
}

void TriangleTest::test_case_copy_constructor() {
  Triangle copy = *m_subject;
  QVERIFY2(*m_subject == copy, "The copy constructor works");
}

#include "tst_triangletest.moc"
