#ifndef JSCONSOLE_H
#define JSCONSOLE_H

#include <QDebug>
#include <QJSEngine>
#include <QKeyEvent>
#include <QObject>

class JSConsole : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
  Q_PROPERTY(bool isDirty READ isDirty WRITE setDirty)

public:
  explicit JSConsole(QObject *parent = nullptr);
  ~JSConsole();
  QString text();
  bool isDirty();
  void setText(const QString&);
  void setDirty(bool);
  void keyPressEvent(QKeyEvent*);

private:
  QString m_text{"Enter text"};
  QString m_lastText;
  QJSEngine m_engine;
  QJSValue m_gloconObj;
  bool m_isDirty = true;
  bool m_hasJustEvaled = false;

signals:
  void textChanged();
  void positionChanged(double, double, double);

public slots:
  QString evaluate();
  void updatePosition(double, double, double);
};

#endif // JSCONSOLE_H
