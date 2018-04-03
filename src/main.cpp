#include <exception>
#include <QApplication>
#include <QCommandLineParser>
#include <QQuickView>
#include <signal.h>

#include "gamestate.h"
#include "jsconsole.h"
#include "noiseviewer.h"
#include "renderer.h"

namespace {

QQuickView* view = nullptr;
GameState* gameState = nullptr;
JSConsole* jsConsole = nullptr;

void handleSig(int signum) {
  Q_UNUSED(signum)
  qDebug() << "We are crashing 😞";
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

  jsConsole = new JSConsole(scriptEngine);
  return jsConsole;
}

//void logToJSConsole(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
//  Q_UNUSED(type)
//  Q_UNUSED(context)
//  jsConsole->setText(msg);
//  jsConsole->textChanged();
//}

} // End anonymous namespace

int main(int argc, char **argv) {
  setupSigHandler();

  QApplication app(argc, argv);
  app.setApplicationName("Glocon");
  app.setApplicationVersion("1.0");
  app.setStartDragDistance(0);

  QCommandLineParser parser;
  parser.setApplicationDescription("A strategy game engine");
  parser.addHelpOption();
  parser.addVersionOption();
  QCommandLineOption qmlMainOption(
    QStringList() << "q" << "qml-main",
    "Use <qml> instead of 'main.qml' in the bundle.",
    "qml"
  );
  parser.addOption(qmlMainOption);

  parser.process(app);

  view = new QQuickView;
  app.setWindowIcon(QIcon(":/assets/icons/app.svg"));

  qmlRegisterType<Renderer>("Glocon", 1, 0, "Renderer");
  qmlRegisterType<NoiseViewer>("Glocon", 1, 0, "NoiseViewer");
  qmlRegisterSingletonType<GameState>("Glocon", 1, 0, "GameState", gameStateProvider);
  qmlRegisterSingletonType<JSConsole>("Glocon", 1, 0, "JSConsole", consoleProvider);

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 1);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSamples(4);

  view->setFormat(format);
  view->rootContext()->setContextProperty("applicationWindow", view);
  view->setTitle("Glocon");
  view->setResizeMode(QQuickView::SizeRootObjectToView);

  if (parser.isSet(qmlMainOption)) {
    QString qmlMain = parser.value(qmlMainOption);
    qDebug() << "Using " << qmlMain << " as qml main";
    view->setSource(qmlMain);
  } else {
    view->setSource(QUrl("qrc:///main.qml"));
  }

  view->show();

  //qInstallMessageHandler(logToJSConsole);

  return app.exec();
}
