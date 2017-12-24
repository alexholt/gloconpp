#include "renderer.h"

Renderer::Renderer() : m_worldMap(":assets/maps/world.svg") {
  connect(this, &QQuickItem::windowChanged, this, &Renderer::handleWindowChanged);
  m_timer.start();
}

void Renderer::setViewportSize(const QSize & size) {
  m_viewportSize = size;
}

void Renderer::handleWindowChanged(QQuickWindow *win) {
  if (win) {
    connect(win, &QQuickWindow::beforeSynchronizing, this, &Renderer::sync, Qt::DirectConnection);
    win->setClearBeforeRendering(false);
  }
}

void Renderer::sync() {
  connect(window(), &QQuickWindow::beforeRendering, this, &Renderer::paint, Qt::DirectConnection);
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

void Renderer::update() {
  qlonglong elapsed = m_timer.elapsed();

  if (elapsed > 0LL) {
    double currentFPS = 1.0F / (double(elapsed) / 1000.0F);
    m_fps += currentFPS / m_fpsUpdateCounter;

    if (m_fpsUpdateCounter == 60) {
      emit fpsChanged(m_fps);
      m_fpsUpdateCounter = 1;
      m_fps = currentFPS;
    }

    m_fpsUpdateCounter++;
  }

  m_timer.restart();
}

void Renderer::paint() {
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
  update();
  window()->update();
}

void Renderer::render(Model& model) {

  //QOpenGLShaderProgram* shader = m_shaders[model.getShaderName()];
  //shader->bind();

  //shader->setUniformValue("cameraMatrix", m_camera.matrix().toTransform());

  //model.getTexture()->bind();

  //shader->enableAttributeArray(model.getName().toStdString().c_str());

  //glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount());
}

QOpenGLTexture* Renderer::createTexture(QImage* image) {
  QOpenGLTexture *texture = new QOpenGLTexture(image->mirrored());
  texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  texture->setMagnificationFilter(QOpenGLTexture::Linear);
  texture->setWrapMode(QOpenGLTexture::ClampToEdge);
  return texture;
}

int Renderer::fps() const {
  return m_fps;
}

void Renderer::setFPS(int val) {
  m_fps = val;
  emit fpsChanged(m_fps);
}
