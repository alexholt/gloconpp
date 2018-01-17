#include <QtTest/QtTest>

#include "tst_territorytest.cpp"
#include "tst_triangletest.cpp"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  qDebug() << "Launching tests";

  TriangleTest testSuite1;
  TerritoryTest testSuite2;

  return QTest::qExec(&testSuite1, argc, argv) |
          QTest::qExec(&testSuite2, argc, argv);
}
