#include <QDebug>
#include <QProcess>
#include <QtTest/QtTest>
#include <QXmlSimpleReader>

#include "glocon.h"
#include "testxmlhandler.h"
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
    int lastResult = QTest::qExec(test, args);
    result |= lastResult;
    QFile resultsFile{"results.xml"};
    resultsFile.open(QIODevice::ReadOnly);
    QString results{resultsFile.readAll()};
    resultsList << results;
    delete test;
  });

  TestXmlHandler handler;
  QXmlSimpleReader xmlReader;
  xmlReader.setContentHandler(&handler);
  xmlReader.setErrorHandler(&handler);

  std::for_each(resultsList.begin(), resultsList.end(), [&] (QString& result) {
    QXmlInputSource source;
    source.setData(result);
    bool ok = xmlReader.parse(source);

    if (!ok) qDebug() << "Parsing failed";
  });

  qInfo() << WHITE << "Executed" << MAGENTA << handler.getTestCount() << WHITE << "tests in"
    << MAGENTA << handler.getDuration() << WHITE << "ms" << RESET;

  return result;
}
