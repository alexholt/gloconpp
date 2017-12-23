#include "jsconsole.h"

JSConsole::JSConsole(QObject* parent) :
  QObject(parent)
{
}

JSConsole::~JSConsole() {
}

QString JSConsole::text() {
  return m_text;
}

void JSConsole::setText(const QString& newText) {
  m_text = newText;
}

QString JSConsole::evaluate() {
  return m_engine.evaluate(m_text).toString();
}
