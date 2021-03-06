#ifndef GLOCON_H
#define GLOCON_H

#include <algorithm>
#include <cmath>
#include <exception>
#include <utility>
#include <QProcess>
#include <QString>
#include <QStringList>

#include "triangle.h"

#define DEFINE_DYNAMICPROP(type, name, defaultValue) \
   public: \
   Q_PROPERTY(type name READ name WRITE name##Setter NOTIFY name##Changed)\
   type name() { return m_##name; }\
   void name##Setter(type name) {\
     if (m_##name != name) {\
       m_##name = name;\
       Q_EMIT name##Changed();\
     }\
   }\
   Q_SIGNAL void name##Changed();\
   protected:\
   type m_##name = defaultValue;\

// Based on boost's hash_combine
// https://stackoverflow.com/a/27952689/545612
constexpr long long hash(const char* str) {
  const char* ptr = str;
  long long val = 0;
  int i = 0;

  while (*ptr != '\0') {
    long long rhs = static_cast<long long>(*ptr) << i % sizeof(long long) * 8;
    val ^= rhs + 0x9e3779b9 + (val >> 2); // The shifting of val and the added constant makes it assymetric
    ptr++;
    i++;
  }

  return val;
}

#define CASE(a) case hash(a)
#define SWITCH(a) switch(hash(a))

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

namespace glocon {

inline QString shell(const QString& cmd, const QStringList& args) {
  QProcess shell;
  shell.start(cmd, args);
  if (!shell.waitForStarted())
    throw std::runtime_error((QString("Cannot execute shell command ") + cmd).toStdString());

  if (!shell.waitForFinished())
    throw std::runtime_error((QString("Shell command did not finish ") + cmd).toStdString());

  QByteArray result = shell.readAll();
  return result;
}

// ø  = arcos( (u . v) / (|u| * |v|) )
inline auto angleBetween(const QVector3D& first, const QVector3D& second) {
  return acos(QVector3D::dotProduct(first, second) / (first.length() * second.length()));
}

inline auto midpoint(const QVector3D& first,  const QVector3D& second) {
  auto x = (first.x() + second.x()) / 2.0f;
  auto y = (first.y() + second.y()) / 2.0f;
  auto z = (first.z() + second.z()) / 2.0f;
  return QVector3D{x, y, z};
}

} // End namespace glocon

#endif // GLOCON_H
