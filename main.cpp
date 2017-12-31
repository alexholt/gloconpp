#include <exception>
#include <QGuiApplication>
#include <QQuickView>
#include <signal.h>

#include "gamestate.h"
#include "jsconsole.h"
#include "renderer.h"

namespace {

GameState* gameState = Q_NULLPTR;
JSConsole* jsConsole = Q_NULLPTR;

void handleSig(int signum) {
  Q_UNUSED(signum)
  throw std::runtime_error("We are crashing 😞");
}

void setupSigHandler() {
  signal(SIGSEGV, handleSig);
}

QObject* gameStateProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  gameState = new GameState();
  return gameState;
}

QObject* consoleProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  jsConsole = new JSConsole();
  return jsConsole;
}

} // End anonymous namespace

int main(int argc, char **argv) {
  setupSigHandler();
  QGuiApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/assets/icons/app.png"));

  qmlRegisterType<Renderer>("Glocon", 1, 0, "Renderer");
  qmlRegisterSingletonType<GameState>("Glocon", 1, 0, "gameState", gameStateProvider);
  qmlRegisterSingletonType<JSConsole>("Glocon", 1, 0, "JSConsole", consoleProvider);

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 1);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);

  QQuickView view;
  view.setFormat(format);
  view.rootContext()->setContextProperty("applicationWindow", &view);
  view.setTitle("Glocon");
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:///main.qml"));
  view.show();

  return app.exec();
}
