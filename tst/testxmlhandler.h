#ifndef TESTXMLHANDLER_H
#define TESTXMLHANDLER_H

class QXmlDefaultHandler;

class TestXmlHandler : public QXmlDefaultHandler {
public:
  TestXmlHandler();
  bool startEntity(const QString&) override;
  bool startElement(const QString&, const QString&, const QString&, const QXmlAttributes&) override;
  bool endElement(const QString&, const QString&, const QString&) override;
  bool characters(const QString&) override;
  int getTestCount() const;
  float getDuration() const;

private:
  QString m_currentTestFunc;
  QString m_failFile;
  QString m_failLine;
  QString m_lastContent;
  bool m_didPass = false;
  bool m_saveNextContent = false;
  int m_testCount = 0;
  float m_duration = 0;
};

#endif // TESTXMLHANDLER_H
