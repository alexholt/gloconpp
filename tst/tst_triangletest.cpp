#include <QtTest>

// add necessary includes here

class triangletest : public QObject
{
  Q_OBJECT

public:
  triangletest();
  ~triangletest();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void test_case1();

};

triangletest::triangletest()
{

}

triangletest::~triangletest()
{

}

void triangletest::initTestCase()
{

}

void triangletest::cleanupTestCase()
{

}

void triangletest::test_case1()
{
  qDebug() << "HELLO";
  QVERIFY(1 == 1);
}

QTEST_APPLESS_MAIN(triangletest)

#include "tst_triangletest.moc"
