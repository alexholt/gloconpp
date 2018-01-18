#ifndef TST_CIRCLETEST_H
#define TST_CIRCLETEST_H

#include <QObject>

class Circle;

class CircleTest : public QObject {
  Q_OBJECT

private:
  Circle* m_subject;

private slots:
  void initTestCase();
  void cleanupTestCase();
  void constructedFromATriangle();
};

#endif // TST_CIRCLETEST_H
