#include "worldmap.h"

WorldMap::WorldMap(const QString& path) : m_path(path) {
}

void WorldMap::forEach(std::function<void(QMap<QString, Territory*>::const_iterator)> func) {
  QMap<QString, Territory*>::const_iterator i = m_territories.constBegin();

  while (i != m_territories.constEnd()) {
    func(i++);
  }
}

WorldMap::~WorldMap() {
  QMap<QString, Territory*>::const_iterator i = m_territories.constBegin();

  while (i != m_territories.constEnd()) {
    delete i.value();
    ++i;
  }
}

void WorldMap::teardownGL() {
  delete m_textureData;
  delete m_tex;
  m_program->release();
  delete m_vao;
  delete m_vertexVbo;
  delete m_textureVbo;
}

void WorldMap::render(QOpenGLFunctions* renderer, const QMatrix4x4& cameraMatrix, const long long elapsed) {
  Q_UNUSED(elapsed)

  if (!m_isInitialized) {
    createTexture();
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/worldmap.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/worldmap.frag");
    m_program->link();
    m_program->bind();

    m_vao->create();
    m_vao->bind();

    m_vertexVbo->create();
    m_vertexVbo->bind();
    m_vertexVbo->allocate(m_vertices, sizeof(m_vertices[0]) * 18);
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    m_textureVbo->create();
    m_textureVbo->bind();
    m_textureVbo->allocate(m_texcoords, sizeof(m_texcoords[0]) * 12);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    m_vao->release();
    m_vertexVbo->release();
    m_textureVbo->release();
    m_program->release();

    m_isInitialized = true;
  }

  m_program->bind();
  m_vao->bind();
  m_texture->bind();

  m_program->setUniformValue("u_camera", cameraMatrix);

  renderer->glDrawArrays(GL_TRIANGLES, 0, sizeof(m_vertices) / sizeof(m_vertices[0]));

  m_vao->release();
  m_texture->release();
  m_program->release();

  auto territories = m_territories.values();
  for (int i = 0; i < territories.length(); i++) {
    territories[i]->setShader("cube");
    territories[i]->render(renderer, cameraMatrix, elapsed);
  }
}

void WorldMap::loadMap() {
  if (m_territories.size() > 0)
    throw std::runtime_error("Map has already been loaded");

  m_doc = QDomDocument(m_path);
  QFile file(m_path);

  if (!file.open(QIODevice::ReadOnly)) {
    QString msg("Unable to find file");
    msg += ' ' + m_path;
    throw std::invalid_argument(msg.toStdString());
  }

  if (!m_doc.setContent(&file)) {
    file.close();
    return;
  }

  file.close();

  QDomElement docElem = m_doc.documentElement();

  for (
    QDomElement territory = docElem.firstChildElement("path");
    !territory.isNull();
    territory = territory.nextSiblingElement("path")
  ) {
    if (!territory.attribute("class").split("/\\s/").contains("ignore")) {
      auto name = territory.attribute("data-name");
      m_territories[name] = new Territory(territory.attribute("d"), name, true);
      m_territories[name]->getMesh();
    }
  }

}

void WorldMap::createTexture() {
  QDomElement docElem = m_doc.documentElement();

  uint i = 0;
  for (
    QDomElement territory = docElem.firstChildElement("path");
    !territory.isNull();
    territory = territory.nextSiblingElement("path")
  ) {
    if (territory.hasAttribute("class") && territory.attribute("class").split("/\\s/").contains("ignore"))
      continue;

    std::stringstream encodedColor;
    encodedColor << "#" << std::setfill('0') << std::setw(6) << std::hex << i;
    territory.setAttribute("fill", encodedColor.str().c_str());
    territory.removeAttribute("inkscape:connector-curvature");
    territory.removeAttribute("data-name");
    territory.removeAttribute("data-id");
    territory.setAttribute("stroke-width", "0");
    ++i;
  }

  QDomElement svg = docElem.firstChildElement("svg");
  svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");
  svg.setAttribute("width", "4000");
  svg.setAttribute("height", "2000");

  QSvgRenderer renderer;
  renderer.load(m_doc.toByteArray());
  m_tex = new QImage(8000, 4000, QImage::Format_ARGB32);
  QPainter painter(m_tex);
  renderer.render(&painter);

  m_texture = new QOpenGLTexture(m_tex->mirrored());
  m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
  m_texture->bind();
}
