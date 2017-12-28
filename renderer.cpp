#include "renderer.h"

const float MOUSE_SCALE = 10000.0f;

Renderer::Renderer() : m_worldMap(":assets/maps/world.svg") {
  //connect(this, &QQuickItem::windowChanged, this, &Renderer::handleWindowChanged);
  m_fpsTimer.setInterval(1000);
  m_fpsTimer.setSingleShot(false);
  connect(&m_fpsTimer, &QTimer::timeout, [=]() {
    emit fpsChanged();
    m_fps = m_frameCount / 1000.0f;
    m_frameCount = 0;
  });
  m_fpsTimer.setTimerType(Qt::PreciseTimer);
  m_fpsTimer.start();
}

void Renderer::setViewportSize(const QSize & size) {
  m_viewportSize = size;
}

void Renderer::setWindow(QQuickWindow *win) {
  m_window = win;
  win->setClearBeforeRendering(false);
  connect(win, &QQuickWindow::beforeSynchronizing, this, &Renderer::sync, Qt::DirectConnection);
  connect(win, &QQuickWindow::beforeRendering, this, &Renderer::paint2, Qt::DirectConnection);
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

void Renderer::paint2() {
  window()->resetOpenGLState();
  m_frameCount++;
  m_fps = m_frameCount;

  if (!m_isInitialized) {
    initializeGL();
    initializeMap();
    m_isInitialized = true;
  }

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
}

void Renderer::onPanX(float x) {
  qDebug() << "panning x " << x;
  m_camera.translate(-x / MOUSE_SCALE, 0, 0);
}

void Renderer::onPanY(float y) {
  qDebug() << "panning y " << y;
  m_camera.translate(0, y / MOUSE_SCALE, 0);
}

void Renderer::zoom(float delta) {
  qDebug() << "zoom delta " << delta;
  m_camera.translate(0, 0, delta / MOUSE_SCALE);
}

