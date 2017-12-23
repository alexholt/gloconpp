#include "worldmap.h"

WorldMap::WorldMap(const QString& path) {
  m_path = path;
}

WorldMap::~WorldMap() {
  QMap<QString, Territory*>::const_iterator i = m_territories.constBegin();

  while (i != m_territories.constEnd()) {
    delete i.value();
    ++i;
  }

  delete m_textureData;
}

void WorldMap::render(QOpenGLFunctions* renderer, const QMatrix4x4& cameraMatrix) {
  if (!m_isInitialized) {
    glActiveTexture(GL_TEXTURE0);
    createTexture();
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/worldmap.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/worldmap.frag");
    m_program->link();
    m_program->bind();

    m_vao.create();
    m_vao.bind();

    m_vertexVbo.create();
    m_vertexVbo.bind();
    m_vertexVbo.allocate(m_vertices, sizeof(m_vertices[0]) * 18);
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    m_textureVbo.create();
    m_textureVbo.bind();
    m_textureVbo.allocate(m_texcoords, sizeof(m_texcoords[0]) * 12);

    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    m_program->setUniformValue("sampler", 0);

    m_vao.release();
    m_vertexVbo.release();
    m_textureVbo.release();
    m_program->release();

    m_isInitialized = true;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_program->bind();

  //m_texObject.bind();
  m_vao.bind();
  glActiveTexture(GL_TEXTURE0);
  m_texture->bind();
  //glActiveTexture(GL_TEXTURE0);

  glDrawArrays(GL_TRIANGLES, 0, sizeof(m_vertices) / sizeof(m_vertices[0]));

  m_vao.release();
  m_texture->release();
  //m_texObject.release();
  m_program->release();
}

QString WorldMap::getShaderName() {
  return "worldmap";
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
    m_territories[territory.attribute("data-name")] = new Territory(territory.attribute("d"));
  }

}

QImage* WorldMap::createTexture() {
  QDomElement docElem = m_doc.documentElement();

  uint i = 0;
  for (
    QDomElement territory = docElem.firstChildElement("path");
    !territory.isNull();
    territory = territory.nextSiblingElement("path")
  ) {
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
  svg.setAttribute("width", "8000");
  svg.setAttribute("height", "4000");

  QSvgRenderer renderer;
  renderer.load(m_doc.toByteArray());
  QImage* tex = new QImage(2000, 1000, QImage::Format_ARGB32);
  QPainter painter(tex);
  renderer.render(&painter);

  m_texture = new QOpenGLTexture(tex->mirrored());
  m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
  glActiveTexture(GL_TEXTURE0);
  m_texture->bind();
  return tex;
}

QString WorldMap::getName() {
  return "worldmap";
}

int WorldMap::getVertexCount() {
  return 6;
}
