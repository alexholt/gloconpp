#include "model.h"

#include <QFile>

Model::Model() :
  m_vao(new QOpenGLVertexArrayObject),
  m_vertexVbo(new QOpenGLBuffer),
  m_elementVbo(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)) {
}

Model::~Model() {
  if (m_vertices != nullptr)
    delete[] m_vertices;

  if (m_elements != nullptr)
    delete[] m_elements;
}

void Model::render(QOpenGLFunctions* renderer, const QMatrix4x4& cameraMatrix, const long long elapsed) {
  Q_UNUSED(cameraMatrix)
  Q_UNUSED(elapsed)

  if (!m_isInitialized) {
    initialize();
  }

  float angle = m_rotationSpeed * static_cast<float>(elapsed);

  m_modelViewMatrix.rotate(angle, 1.0, 1.0, 1.0);

  m_program->bind();
  m_vao->bind();

  m_program->setUniformValue("u_camera", cameraMatrix);
  m_program->setUniformValue("u_modelView", m_modelViewMatrix);

  renderer->glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_SHORT, 0);

  m_vao->release();
  m_program->release();
}

void Model::initialize() {
  m_program = new QOpenGLShaderProgram;
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/" + m_shaderName + ".vert");
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/" + m_shaderName + ".frag");
  m_program->link();
  m_program->bind();

  m_vao->create();
  m_vao->bind();

  m_vertexVbo->create();
  m_vertexVbo->bind();
  m_vertexVbo->allocate(m_vertices, m_numVertices * sizeof(float));
  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

  m_elementVbo->create();
  m_elementVbo->bind();
  m_elementVbo->allocate(m_elements, m_numElements * sizeof(ushort));

  m_vao->release();
  m_vertexVbo->release();
  m_elementVbo->release();
  m_program->release();

  m_isInitialized = true;
}

GLuint Model::createProgram(QOpenGLFunctions* renderer, QString& vert, QString& frag) {
  GLuint program = renderer->glCreateProgram();
  GLuint vertShader = createShader(renderer, GL_VERTEX_SHADER, vert);
  GLuint fragShader = createShader(renderer, GL_FRAGMENT_SHADER, frag);

  renderer->glAttachShader(program, vertShader);
  renderer->glAttachShader(program, fragShader);
  renderer->glLinkProgram(program);

  GLint* params = new GLint;
  renderer->glGetProgramiv(program, GL_LINK_STATUS, params);
  if (!*params) {
    int max = 512;
    GLsizei* length = new GLsizei;
    char* infolog = new char[max];
    renderer->glGetProgramInfoLog(program, max, length, infolog);
    renderer->glDeleteProgram(program);
    auto err = std::runtime_error(infolog);
    delete length;
    delete params;
    delete[] infolog;

    throw err;
  }

  return program;
}

GLuint Model::createShader(QOpenGLFunctions* renderer, GLenum type, QString& source) {
  GLuint shader = renderer->glCreateShader(type);
  const std::string stdStr = source.toStdString();
  const char* sourceData = stdStr.c_str();
  renderer->glShaderSource(shader, 1, &sourceData, 0);
  renderer->glCompileShader(shader);

  GLint* params = new GLint;
  renderer->glGetShaderiv(shader, GL_COMPILE_STATUS, params);
  if (!*params) {
    int max = 512;
    GLsizei* length = new GLsizei;
    char* infolog = new char[max];
    renderer->glGetShaderInfoLog(shader, max, length, infolog);
    renderer->glDeleteShader(shader);

    QString typeStr("fragment");
    if (GL_VERTEX_SHADER == type)
      QString typeStr("vertex");

    auto err = std::runtime_error((QString("Problem creating ") + typeStr + " " + infolog).toStdString());
    delete length;
    delete params;
    delete[] infolog;

    throw err;
  }

  return shader;
}

void Model::translate(float x, float y, float z) {
  m_modelViewMatrix.translate(x, y, z);
}

void Model::loadFile(const QString& modelFilePath, const QString& shaderName) {
  m_shaderName = shaderName;

  Assimp::Importer importer;

  QFile modelFile(modelFilePath);
  modelFile.open(QIODevice::ReadOnly);

  QString ext(modelFilePath.split('.')[1]);

  if (!importer.IsExtensionSupported(ext.toUtf8())) {
    QString msg("Extension is not supported ");
    msg += ext.toUtf8();
    throw std::runtime_error(msg.toStdString());
  }

  auto bytes = modelFile.readAll();
  const aiScene* scene = importer.ReadFileFromMemory(
    bytes.data_ptr(),
    bytes.length(),
    aiProcess_Triangulate | aiProcess_JoinIdenticalVertices,
    modelFilePath.split('.')[1].toUtf8()
  );

  if (!scene) {
    QString msg;
    msg += "Couldn't read scene from file: " + modelFilePath + importer.GetErrorString();
    throw std::runtime_error(msg.toStdString());
  }

  if (scene->mNumMeshes > 1)
    qDebug() << "Warning: the obj file contains more than one mesh. Reading only the first one";

  if (scene->mNumMeshes == 0)
    throw std::runtime_error((QString("File contains no meshes: ") + modelFilePath).toStdString());

  aiMesh* mesh = scene->mMeshes[0];
  m_meshData.elementCount = mesh->mNumFaces * 3;

  m_numVertices = mesh->mNumVertices * 8;
  m_vertices = new float[m_numVertices];
  for (uint i = 0; i < mesh->mNumVertices; i++) {
    uint p = i * 3;
    m_vertices[p] = mesh->mVertices[i][0];
    m_vertices[p + 1] = mesh->mVertices[i][1];
    m_vertices[p + 2] = mesh->mVertices[i][2];

    if (mesh->HasTextureCoords(0)) {
      m_vertices[p + 3] = mesh->mTextureCoords[0][i].x;
      m_vertices[p + 4] = mesh->mTextureCoords[0][i].y;
    } else {
      m_vertices[p + 3] = 0.0f;
      m_vertices[p + 4] = 0.0f;
    }

    if (mesh->HasNormals()) {
      m_vertices[p + 5] = mesh->mNormals[i][0];
      m_vertices[p + 6] = mesh->mNormals[i][1];
      m_vertices[p + 7] = mesh->mNormals[i][2];
    } else {
      m_vertices[p + 5] = 0.0f;
      m_vertices[p + 6] = 0.0f;
      m_vertices[p + 7] = 0.0f;
    }
  }


  m_numElements = mesh->mNumFaces * 3;
  m_elements = new ushort[m_numElements];

  for (uint i = 0; i < mesh->mNumFaces; i++) {
    m_elements[i * 3] = mesh->mFaces[i].mIndices[0];
    m_elements[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
    m_elements[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
  }

  scale(10.0f);
}

void Model::scale(float factor) {
  m_modelViewMatrix.scale(factor);
}
