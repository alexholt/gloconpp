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
}

double Renderer::fps() const {
  return m_fps;
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

  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

  //if (m_lastZoom != m_camera.position()->z())
  //m_camera.translate(0, 0, m_lastZoom);
  //QPoint delta = m_mousePoint - m_lastMousePoint;
  //m_camera.translate(delta.x(), delta.y(), 0);

  glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
  glClearColor(0, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  QMatrix4x4* camera = m_camera.matrix();
  m_worldMap.render(this, *camera);

  window()->resetOpenGLState();
  window()->update();
}

QOpenGLTexture* Renderer::createTexture(QImage* image) {
  QOpenGLTexture *texture = new QOpenGLTexture(image->mirrored());
  texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  texture->setMagnificationFilter(QOpenGLTexture::Linear);
  texture->setWrapMode(QOpenGLTexture::ClampToEdge);
  return texture;
}

void Renderer::onKeyPressed(Qt::Key key) {
  qDebug() << "This key was pressed " << key;
  if (key == Qt::Key_Tab) {
    if (window()->windowState() == Qt::WindowFullScreen)
      window()->showNormal();
    else
      window()->showFullScreen();
  }
}

void Renderer::onPanX(float x) {
  m_lastMousePoint.setX(m_mousePoint.x());
  m_mousePoint.setX(x);
}

void Renderer::onPanY(float y) {
  m_lastMousePoint.setY(m_mousePoint.y());
  m_mousePoint.setY(y);
}

void Renderer::zoom(QPoint point) {
  qDebug() << point;
  qDebug() << point.y();
  if (point.y() < 0)
    m_camera.translate(0, 0, -10);
  else
    m_camera.translate(0, 0, 10);
  //m_lastZoom = point.y();
}
