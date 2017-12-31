#include "jsconsole.h"

JSConsole::JSConsole(QObject* parent) :
  QObject(parent)
{
  QJSValue global = m_engine.globalObject();
  m_gloconObj = m_engine.newObject();
  m_gloconObj.setProperty("x", 0);
  m_gloconObj.setProperty("y", 0);
  m_gloconObj.setProperty("z", 0);
  global.setProperty("glocon", m_gloconObj);
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

#define updateProp(prop) m_gloconObj.setProperty(#prop, prop);

void JSConsole::updatePosition(double x, double y, double z) {
  qDebug() << "updating" << x << y << z;
  updateProp(x);
  updateProp(y);
  updateProp(z);
}

#undef update
