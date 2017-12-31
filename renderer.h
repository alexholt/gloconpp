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
#include <QRect>
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
  Q_PROPERTY(double x READ x)
  Q_PROPERTY(double y READ y)
  Q_PROPERTY(double z READ z)
  Q_PROPERTY(QQuickWindow* window READ window WRITE setWindow)
  Q_PROPERTY(QRect* contentRect READ contentRect NOTIFY contentRectChanged)

public:
  Renderer();
  ~Renderer();
  void setViewportSize(const QSize &);
  QQuickWindow* window() const;
  void setWindow(QQuickWindow*);
  double fps() const;
  QRect* contentRect();
  double x();
  double y();
  double z();

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
  QRect m_contentRect{0, 0, 2000, 1000};
  QQuickWindow* m_window;
  int m_frameCount = 0;
  QPoint m_mousePoint;
  QPoint m_lastMousePoint;
  void printGLInfo();
  double m_lastZoom = 0;

signals:
  void fpsChanged();
  void windowChanged();
  void contentRectChanged();

public slots:
  void sync();
  void paint();
  void onKeyPressed(Qt::Key);
  void onPanX(float);
  void onPanY(float);
  void updatePosition(double, double, double);
  void teardownGL();
};

#endif // VIEWPORT_H
