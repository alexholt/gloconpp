#ifndef GLOCON_H
#define GLOCON_H

#include <algorithm>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <exception>

namespace glocon {

QString shell(const QString& cmd, const QStringList& args = QStringList());

} // End namespace glocon

#endif // GLOCON_H
