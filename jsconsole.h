#ifndef JSCONSOLE_H
#define JSCONSOLE_H

#include <QDebug>
#include <QJSEngine>
#include <QKeyEvent>
#include <QObject>

class JSConsole : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
  explicit JSConsole(QObject *parent = nullptr);
  ~JSConsole();
  QString text();
  void setText(const QString& newText);
  void keyPressEvent(QKeyEvent* event);

private:
  QString m_text{"Enter text"};
  QString m_lastText;
  QJSEngine m_engine;
  bool m_hasJustEvaled = false;

signals:
  void textChanged();

public slots:
  QString evaluate();
};

#endif // JSCONSOLE_H
