#ifndef TST_TRIANGLETEST_H
#define TST_TRIANGLETEST_H

#include <QObject>

class Triangle;

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
  void testCaseisValid();
  void testCaseContains();
};

#endif // TST_TRIANGLETEST_H
