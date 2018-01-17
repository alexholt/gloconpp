#include "glocon.h"

namespace glocon {

QString shell(const QString& cmd, const QStringList& args) {
  QProcess shell;
  shell.start(cmd, args);
  if (!shell.waitForStarted())
    throw std::runtime_error((QString("Cannot execute shell command ") + cmd).toStdString());

  if (!shell.waitForFinished())
    throw std::runtime_error((QString("Shell command did not finish ") + cmd).toStdString());

  QByteArray result = shell.readAll();
  return result;
}

} // End namespace glocon
