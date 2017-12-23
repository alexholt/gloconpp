#ifndef GLOCON_H
#define GLOCON_H

#include <QProcess>
#include <QString>
#include <QStringList>
#include <exception>

namespace glocon {

QString shell(const QString& cmd, const QStringList& args = QStringList());

}

#endif // GLOCON_H
