#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QDomDocument>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickItem>
#include <QQuickWindow>

#include "camera.h"
#include "worldmap.h"

class Renderer : public QQuickItem, public QOpenGLFunctions {
  Q_OBJECT

public:
  Renderer();
  void setViewportSize(const QSize &);
  QOpenGLTexture* createTexture(QImage*);

private:
  void initializeGL();
  void initializeMap();
  void render(Model&);
  QSize m_viewportSize;
  QOpenGLShaderProgram *m_program;
  WorldMap m_worldMap;
  Camera m_camera;
  bool m_isInitialized = false;
  QMap<QString, QOpenGLShaderProgram*> m_shaders;

signals:
  void tChanged();

public slots:
  void sync();
  void paint();

private slots:
  void handleWindowChanged(QQuickWindow *win);
};

#endif // VIEWPORT_H
