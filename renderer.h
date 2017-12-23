#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QDateTime>
#include <QDomDocument>
#include <QElapsedTimer>
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
  Q_PROPERTY (int fps READ fps WRITE setFPS NOTIFY fpsChanged)

public:
  Renderer();
  void setViewportSize(const QSize &);
  QOpenGLTexture* createTexture(QImage*);
  void update();
  void setFPS(int fps);
  int fps() const;

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
  QElapsedTimer m_timer;
  double m_fps = 60;
  int m_fpsUpdateCounter = 1;

signals:
  void fpsChanged(int);

public slots:
  void sync();
  void paint();

private slots:
  void handleWindowChanged(QQuickWindow *win);
};

#endif // VIEWPORT_H
