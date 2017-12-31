#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QDateTime>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QPoint>
#include <QQmlContext>
#include <QQuickWindow>
#include <QThread>
#include <QTimer>
#include <QWheelEvent>

#include "camera.h"
#include "worldmap.h"

class Renderer : public QObject, public QOpenGLFunctions {
  Q_OBJECT
  Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
  Q_PROPERTY(QQuickWindow* window READ window WRITE setWindow)

public:
  Renderer();
  void setViewportSize(const QSize &);
  QOpenGLTexture* createTexture(QImage*);
  QQuickWindow* window() const;
  void setWindow(QQuickWindow*);
  double fps() const;

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
  QPoint m_mousePoint;
  QPoint m_lastMousePoint;
  void printGLInfo();

signals:
  void fpsChanged();
  void windowChanged();

public slots:
  void sync();
  void paint();
  void onKeyPressed(Qt::Key);
  void onPanX(float);
  void onPanY(float);
  void zoom(QPoint);
  void teardownGL();
};

#endif // VIEWPORT_H
