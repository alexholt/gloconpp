#include "jsconsole.h"

JSConsole::JSConsole(QObject* parent) : QObject(parent) {
  m_engine.installExtensions(QJSEngine::AllExtensions);
  QJSValue global = m_engine.globalObject();
  m_gloconObj = m_engine.newObject();
  global.setProperty("glocon", m_gloconObj);

  QFile script(":/assets/scripts/glocon.js");
  if (!script.open(QIODevice::ReadOnly)) {
    throw std::invalid_argument("Unable to find glocon.js");
  }

  m_engine.evaluate(script.readAll());
}

JSConsole::~JSConsole() {
}

QString JSConsole::text() {
  return m_text;
}

void JSConsole::setText(const QString& newText) {
  m_text = newText;
}

void JSConsole::setDirty(const bool isDirty) {
  m_isDirty = isDirty;
}

bool JSConsole::isDirty() {
  return m_isDirty;
}

QString JSConsole::evaluate() {
  QString result = m_engine.evaluate(m_text).toString();
  emit positionChanged(
    m_gloconObj.property("x").toNumber(),
    m_gloconObj.property("y").toNumber(),
    m_gloconObj.property("z").toNumber()
  );
  return result;
}

void JSConsole::updatePosition(double x, double y, double z) {
  #define updateProp(prop) m_gloconObj.setProperty(#prop, prop);
  updateProp(x);
  updateProp(y);
  updateProp(z);
  #undef updateProp
}
