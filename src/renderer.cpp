#include "renderer.h"

#include "glocon.h"

Renderer::Renderer() : m_worldMap(":assets/maps/just-us.svg"), m_tank(false) {
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

  m_tank.loadFile(":/assets/models/tank.obj", "cube");
  m_tank.scale(10.0f);
  m_tank.translate(10.0f, 0.0f, 0.0f);
  m_tank.rotate(90.0f, 1.0f, 0.0f, 0.0f);

  m_mars.loadFile(":/assets/models/mars.obj");
  m_mars.scale(10.0f);
  m_mars.translate(-20.0f, 20.0f, 30.0f);
  m_mars.scale(10.0f);

  m_planes[0].loadFile(":/assets/models/plane.obj", "fuzzycircle");
  m_planes[0].scale(10.0f);
  m_planes[0].translate(10.0f, 0.0f, 20.0f);
  m_planes[0].scale(10.0f);

  m_planes[1].loadFile(":/assets/models/plane.obj", "diffuse");
  m_planes[1].scale(10.0f);
  m_planes[1].translate(50.0f, 0.0f, 20.0f);
  m_planes[1].scale(10.0f);

  m_monkey.loadFile(":/assets/models/monkey.obj", "multiads");
  m_monkey.translate(-500.0f, 200.0f, 200.0f);
  m_monkey.scale(100.0f);
  m_monkey.setShouldRotate(false);

  m_lattice.setShader("ads");
  m_lattice.buildMesh();
  for (int i = 0; i < 11; i++) {
    m_lattice.subdivide();
  }

  m_lights << Light{QVector3D{0.8f, 0.8f, 0.0f}}
    << Light{QVector3D{0.0f, 0.0f, 0.8f}};

  m_lights[0].translate(-400.0f, 200.0f, 200.0f);
  m_lights[1].translate(-600.0f, 200.0f, 200.0f);
}

Renderer::~Renderer() {
  for (int i = 0; i < m_cubeList.length(); i++)
    delete m_cubeList[i];
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
  double width = static_cast<double>(windowSize.width());
  double height = static_cast<double>(windowSize.height());
  m_camera.setAspectRatio(width, height);

  // A different thread will run paint() so we need to copy everything over that will be used there
  delete m_renderCameraMatrix;
  m_renderCameraMatrix = new QMatrix4x4(*m_camera.matrix());
}

void Renderer::initializeGL() {
  initializeOpenGLFunctions();
  printGLInfo();

  QOpenGLContext* context = window()->openglContext();
  connect(context, SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
  //connect(context, SIGNAL(aboutToBeDestroyed()), &m_worldMap, WorldMap::teardownGL, Qt::DirectConnection);

  auto surface = window()->format();
  surface.setSwapInterval(0);

  m_paintTimer.start();
}

void Renderer::teardownGL() {
  qDebug() << "Goodbye 👋";
}

void Renderer::initializeMap() {
  m_worldMap.loadMap();

  //float height = 5;
  //m_worldMap.forEach([this, &height](QMap<QString, Territory*>::const_iterator it) {
  //  auto cube = new Cube();
  //  cube->translate(it.value()->getCentroid()->x() - 1000, -it.value()->getCentroid()->y() + 500, height++);
  //  cube->scale(10);
  //  m_cubeList << cube;
  //});

  m_lattice.intersection(m_worldMap.territory("United States"));
  //m_lattice.rotate(90.0f, 1.0f, 0.0f, 0.0f);
  m_worldMap.removeTerritory("United States");
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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);

  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_MULTISAMPLE);

  long long elapsed = m_paintTimer.restart();

  glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
  glClearColor(0, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_worldMap.render(this, *m_renderCameraMatrix, elapsed);
  m_lattice.render(this, *m_renderCameraMatrix, elapsed);

  for (int i = 0; i < m_cubeList.length(); i++) {
    m_cubeList[i]->render(this, *m_renderCameraMatrix, elapsed);
  }

  m_tank.render(this, *m_renderCameraMatrix, elapsed);
  m_mars.render(this, *m_renderCameraMatrix, elapsed);
  m_planes[0].render(this, *m_renderCameraMatrix, elapsed);
  m_planes[1].render(this, *m_renderCameraMatrix, elapsed);

  auto program = m_monkey.program();

  if (program) {
    program->bind();
    program->setUniformValue("lights[0].position", m_lights[0].matrix());
    program->setUniformValue("lights[0].intensity", m_lights[0].color());
    program->setUniformValue("lights[1].position", m_lights[1].matrix());
    program->setUniformValue("lights[1].intensity", m_lights[1].color());
  }

  m_monkey.render(this, *m_renderCameraMatrix, elapsed);

  window()->update();
  window()->resetOpenGLState();
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

double Renderer::onPanX(float x) {
  m_lastMousePoint.setX(m_mousePoint.x());
  m_mousePoint.setX(x);
  auto deltaX = m_mousePoint.x() - m_lastMousePoint.x();
  qDebug() << "Panning" << deltaX;
  return 1367.0;
}

double Renderer::onPanY(float y) {
  m_mousePoint.setY(y);
  return 800.0;
}

void Renderer::updatePosition(double x, double y, double z) {
  auto zoom = m_camera.scaleToZ(z);
  m_camera.moveTo(x, y, zoom);
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

#define GET_PROP(prop, type) \
data.property(prop).to##type()

#define CHECK_PROP(prop, type) \
if (data.hasProperty(prop) && data.property(prop).is##type()) { \
  auto PROP = GET_PROP(prop, type);

#define END_CHECK }

QString Renderer::receiveUpdate(QJSValue data) {
  if (data.isString())
    qDebug() << "We have received the update: [" << data.toString() << "]";

  if (!data.isObject())
    qWarning() << "Updates should be objects";

  CHECK_PROP("shouldRotate", Bool)
    m_monkey.setShouldRotate(PROP);
  END_CHECK

  CHECK_PROP("echo", String)
    qDebug() << PROP;
  END_CHECK

  CHECK_PROP("activeShader", String)
    m_monkey.setShader(PROP);
  END_CHECK

  CHECK_PROP("lightOneIntensity", Number)
    auto prop = static_cast<float>(PROP);
    m_lights[0].setIntensity(prop);
  END_CHECK

  CHECK_PROP("lightTwoIntensity", Number)
    auto prop = static_cast<float>(PROP);
    m_lights[1].setIntensity(prop);
  END_CHECK

  return "We have received the update";
}

#undef CHECK_PROP
#undef GET_PROP
