#include <exception>
#include <QGuiApplication>
#include <QQuickView>
#include <signal.h>

#include "jsconsole.h"
#include "renderer.h"

void handleSig(int signum) {
  throw std::runtime_error("We are crashing 😞");
}

void setupSigHandler() {
  signal(SIGSEGV, handleSig);
}

int main(int argc, char **argv) {
  setupSigHandler();
  QGuiApplication app(argc, argv);

  JSConsole console;
  QQuickView view;

  qmlRegisterType<Renderer>("GloconPP", 1, 0, "Viewport");
  qmlRegisterType<JSConsole>("GloconPP", 1, 0, "JSConsole");

  //view.showFullScreen();
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 1);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);

  view.setFormat(format);

  view.setTitle("Glocon");
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:///main.qml"));
  view.show();

  return app.exec();
}
