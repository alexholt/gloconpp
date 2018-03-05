#include "gamestate.h"

#include <QDebug>

GameState::GameState() {}

GameState::~GameState() {}

void GameState::hello() {
  qDebug() << "Hello from gameState";
}
