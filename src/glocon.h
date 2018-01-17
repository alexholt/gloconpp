#ifndef GLOCON_H
#define GLOCON_H

#include <algorithm>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <exception>

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

namespace glocon {

QString shell(const QString& cmd, const QStringList& args = QStringList());

} // End namespace glocon

#endif // GLOCON_H
