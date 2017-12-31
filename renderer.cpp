#include "renderer.h"

const float MOUSE_SCALE = 10000.0f;

Renderer::Renderer() : m_worldMap(":assets/maps/world.svg") {
  m_fpsTimer.setInterval(1000);
  m_fpsTimer.setSingleShot(false);
  m_fpsTimer.setTimerType(Qt::PreciseTimer);
  connect(&m_fpsTimer, &QTimer::timeout, [=]() {
    emit fpsChanged();
    m_fps = m_frameCount / 1000.0f;
    m_frameCount = 0;
  });
  m_fpsTimer.start();
  emit contentRectChanged();
}

Renderer::~Renderer() {
}

double Renderer::fps() const {
  return m_fps;
}

QRect* Renderer::contentRect() {
  return &m_contentRect;
}

void Renderer::setViewportSize(const QSize & size) {
  m_viewportSize = size;
}

void Renderer::setWindow(QQuickWindow *win) {
  m_window = win;
  win->setClearBeforeRendering(false);
  connect(win, &QQuickWindow::beforeSynchronizing, this, &Renderer::sync, Qt::DirectConnection);
  connect(win, &QQuickWindow::beforeRendering, this, &Renderer::paint, Qt::DirectConnection);
}

QQuickWindow* Renderer::window() const {
  return m_window;
}

void Renderer::sync() {
  auto windowSize = window()->size();
  setViewportSize(windowSize * window()->devicePixelRatio());
  qreal width = windowSize.width();
  qreal height = windowSize.height();
  m_camera.setAspectRatio(width / height);
}

void Renderer::initializeGL() {
  initializeOpenGLFunctions();
  printGLInfo();

  QOpenGLContext* context = window()->openglContext();
  connect(context, SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
  connect(context, SIGNAL(aboutToBeDestroyed()), &m_worldMap, SLOT(teardownGL()), Qt::DirectConnection);

  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::teardownGL() {
  qDebug() << "Goodbye 👋";
}

void Renderer::initializeMap() {
  m_worldMap.loadMap();
}

void Renderer::paint() {
  window()->resetOpenGLState();
  m_frameCount++;
  m_fps = m_frameCount;

  if (!m_isInitialized) {
    initializeGL();
    initializeMap();
    m_isInitialized = true;
  }

  m_camera.translate(0, 0, m_lastZoom);
  m_lastZoom = 0;
  //float scale = -m_camera.position().z();
  m_camera.translate(m_mousePoint.x(), 0, 0);
  m_mousePoint.setX(0);

  glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
  glClearColor(0, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  QMatrix4x4* camera = m_camera.matrix();
  m_worldMap.render(this, *camera);

  window()->resetOpenGLState();
  window()->update();
}

void Renderer::onKeyPressed(Qt::Key key) {
  switch (key) {
    case Qt::Key_Tab:
      if (window()->windowState() == Qt::WindowFullScreen)
        window()->showNormal();
      else
        window()->showFullScreen();
      break;
    case Qt::Key_Escape:
      window()->close();
      break;
    default:
      qDebug() << "This key was pressed " << key;
  }
}

void Renderer::onPanX(float x) {
  //m_lastMousePoint.setX(m_mousePoint.x());
  m_mousePoint.setX(-x);
}

void Renderer::onPanY(float y) {
  m_lastMousePoint.setY(m_mousePoint.y());
  m_mousePoint.setY(y);
}

void Renderer::updatePosition(double x, double y, double z) {
  m_camera.moveTo(x, y, z);
}

void Renderer::printGLInfo() {
  QString glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << "OpenGL" << qPrintable(glVersion);
}

double Renderer::x() {
  return m_camera.position().x();
}

double Renderer::y() {
  return m_camera.position().y();
}

double Renderer::z() {
  return m_camera.position().z();
}
