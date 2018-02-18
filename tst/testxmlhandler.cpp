#include <QDebug>
#include <QXmlDefaultHandler>

#include "testxmlhandler.h"
#include "glocon.h"

TestXmlHandler::TestXmlHandler() {
}

bool TestXmlHandler::startEntity(const QString& name) {
  qDebug() << name;
  return true;
}

bool TestXmlHandler::startElement(const QString& namespaceUri, const QString& localName, const QString& qName, const QXmlAttributes& atts) {
  Q_UNUSED(qName)
  Q_UNUSED(namespaceUri)

  SWITCH(localName.toLatin1()) {

    CASE("TestCase"):
      qInfo() << CYAN << qPrintable(atts.value("name")) << RESET;
      break;

    CASE("TestFunction"):
      m_testCount++;
      m_currentTestFunc = atts.value("name");
      break;

    CASE("Incident"): {
      auto type = atts.value("type");
      if (type == "pass") {
        m_didPass = true;
      } else if (type == "fail") {
        m_failFile = atts.value("file");
        m_failLine = atts.value("line");
      }
      break;
    }

    CASE("Description"):
      m_saveNextContent = true;
      break;

    CASE("Duration"):
      m_duration += atts.value("msecs").toFloat();
      break;
  }

  return true;
}

bool TestXmlHandler::endElement(const QString& namespaceUri, const QString& localName, const QString& qName) {
  Q_UNUSED(namespaceUri)
  Q_UNUSED(qName)

  SWITCH(localName.toLatin1()) {
    CASE("TestFunction"):
      qInfo() << (m_didPass ? " ✅ " : " ❌ ") << (m_didPass ? GREEN : RED) << qPrintable(m_currentTestFunc);
      if (!m_didPass) {
        qInfo() << WHITE << "   " << qPrintable(m_failFile) << ':' << qPrintable(m_failLine);
        qInfo() << WHITE << "   " << qPrintable(m_lastContent);
      }
      m_didPass = false;
      break;

    CASE("Message"):
      qInfo() << WHITE << m_lastContent;
      break;
  }
  return true;
}

bool TestXmlHandler::characters(const QString& content) {
  if (m_saveNextContent) {
    m_lastContent = content;
    m_saveNextContent = false;
  }
  return true;
}

int TestXmlHandler::getTestCount() const {
  return m_testCount;
}

float TestXmlHandler::getDuration() const {
  return m_duration;
}
