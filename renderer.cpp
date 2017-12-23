#include "renderer.h"

Renderer::Renderer() : m_worldMap(":assets/maps/world.svg") {
  connect(this, &QQuickItem::windowChanged, this, &Renderer::handleWindowChanged);
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
  m_camera.setAspectRatio(windowSize.width() / windowSize.height());
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
  if (!m_isInitialized) {
    initializeGL();
    initializeMap();
    m_isInitialized = true;
  }

  glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
  glClearColor(0, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_worldMap.render(this, m_camera.matrix().toTransform());

  window()->resetOpenGLState();
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
