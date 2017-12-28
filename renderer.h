#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QDateTime>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>

#include "camera.h"
#include "worldmap.h"

class Renderer : public QObject, public QOpenGLFunctions {
  Q_OBJECT
  Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
  Q_PROPERTY(QQuickWindow* window READ window WRITE setWindow NOTIFY windowChanged)

public:
  Renderer();
  void setViewportSize(const QSize &);
  QOpenGLTexture* createTexture(QImage*);
  QQuickWindow* window() const;
  void setWindow(QQuickWindow*);

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
  QTimer m_fpsTimer;
  double m_fps = 60;
  QQuickWindow* m_window;
  int m_frameCount = 0;

signals:
  void fpsChanged();
  void windowChanged();

public slots:
  double fps() { return m_fps; }
  void sync();
  void paint2();
  void onKeyPressed(Qt::Key);
  void onPanX(float);
  void onPanY(float);
  void zoom(float);
};

#endif // VIEWPORT_H
