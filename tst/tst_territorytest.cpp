#include <QtTest>

#include "territory.h"

class TerritoryTest : public QObject {
  Q_OBJECT

private:
  Territory* m_subject;

private slots:
  void initTestCase();
  void cleanupTestCase();
  void test_case_getMesh();
};

void TerritoryTest::initTestCase() {
  m_subject = new Territory("m 1068.3,609.6 -16.6,-0.1 -1.9,0.7 -1.7,-0.1 z");
}

void TerritoryTest::cleanupTestCase() {
  delete m_subject;
}

void TerritoryTest::test_case_getMesh() {

}

#include "tst_territorytest.moc"
