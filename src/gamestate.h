#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>

class GameState : public QObject {
  Q_OBJECT

public:
  GameState();
  ~GameState();

public slots:
  QString hello();
};

#endif // GAMESTATE_H

