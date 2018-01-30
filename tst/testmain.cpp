#include <QDebug>
#include <QProcess>
#include <QtTest/QtTest>

#include "tst_circletest.h"
#include "tst_territorytest.h"
#include "tst_triangletest.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  QList<QObject*> tests;
  tests << new CircleTest;
  tests << new TriangleTest;
  tests << new TerritoryTest;

  int result = 0;

  QStringList args;
  args << " " << "-xml" << "-o" << "results.xml";

  QStringList resultsList;

  std::for_each(tests.begin(), tests.end(), [&] (QObject* test) {
    result |= QTest::qExec(test, args);
    QFile resultsFile{"results.xml"};
    resultsFile.open(QIODevice::ReadOnly);
    QString results{resultsFile.readAll()};
    resultsList << results;
    delete test;
  });


  qDebug() << resultsList;

  return result;
}
