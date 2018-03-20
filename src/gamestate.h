#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QJSValue>
#include <QObject>

class GameState : public QObject {
  Q_OBJECT

public:
  GameState();
  ~GameState();

public slots:
  void hello();

signals:
  void sendUpdate(QJSValue);

};

#endif // GAMESTATE_H

