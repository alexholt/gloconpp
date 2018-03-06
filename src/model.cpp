#include "model.h"

#include <QFile>

#include "glocon.h"
#include "triangle.h"

Model::Model() : Model(false) {}

Model::Model(bool hasTexture) :
  m_vao(new QOpenGLVertexArrayObject),
  m_vertexVbo(new QOpenGLBuffer),
  m_elementVbo(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)),
  m_textureVbo(new QOpenGLBuffer()) {

  m_hasTexture = hasTexture;
}

Model::~Model() {
  if (m_vertices != nullptr)
    delete[] m_vertices;

  if (m_elements != nullptr)
    delete[] m_elements;
}

Model::Model(const Model& other) : Model(other.m_hasTexture) {
  m_shaderName = other.m_shaderName;
}

void Model::render(QOpenGLFunctions_4_1_Core* renderer, const QMatrix4x4& cameraMatrix, const long long elapsed) {
  if (!m_isInitialized) {
    initialize(renderer);
  }

  if (m_shouldRotate) {
    float angle = m_rotationSpeed * static_cast<float>(elapsed);
    m_modelViewMatrix.rotate(angle, 1.0, 1.0, 1.0);
  }

  m_program->bind();
  m_vao->bind();

  if (m_hasTexture)
    m_texture->bind();

  setUniforms(cameraMatrix, *renderer);
  renderer->glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_SHORT, 0);

  m_vao->release();
  m_program->release();


  if (m_hasTexture)
    m_texture->release();
}

void Model::setUniforms(const QMatrix4x4& cameraMatrix, QOpenGLFunctions_4_1_Core& renderer) {
  m_program->setUniformValue("u_camera", cameraMatrix);
  m_program->setUniformValue("u_modelView", m_modelViewMatrix);

  const char* shaderName = m_shaderName.toLatin1();
  SWITCH(shaderName) {
    CASE("fuzzycircle"): {
      m_program->setUniformValue("u_innerColor", QVector4D{0.0, 0.0, 0.0, 0.3});
      m_program->setUniformValue("u_outerColor", QVector4D{1.0, 1.0, 1.0, 1.0});
      m_program->setUniformValue("u_radiusInner", 0.25f);
      m_program->setUniformValue("u_radiusOuter", 0.45f);
      break;
    }

    CASE("diffuse"): {
      m_program->setUniformValue("u_lightPosition", QVector4D{0.0, 0.0, 1000.0, 1.0});
      m_program->setUniformValue("u_kd", QVector3D{1.0, 1.0, 1.0});
      m_program->setUniformValue("u_ld", QVector3D{0.5, 0.5, 1.0});
      break;
    }

    CASE("ads"): {
      m_program->setUniformValue("light.position", QVector4D{0.0, 0.0, 1000.0, 1.0});
      m_program->setUniformValue("light.la", QVector3D{1.0, 0.5, 0.5});
      m_program->setUniformValue("light.ld", QVector3D{0.5, 0.5, 0.5});
      m_program->setUniformValue("light.ls", QVector3D{0.5, 0.5, 0.5});

      m_program->setUniformValue("material.ka", QVector3D{0.5, 0.5, 0.5});
      m_program->setUniformValue("material.kd", QVector3D{0.8, 0.8, 0.2});
      m_program->setUniformValue("material.ks", QVector3D{0.5, 0.5, 0.5});
      m_program->setUniformValue("material.shininess", 0.001f);

      //GLuint subIndex = glGetSubroutineIndex(m_program->programId(), GL_VERTEX_SHADER, "diffuseOnly");
      GLuint subIndex = renderer.glGetSubroutineIndex(m_program->programId(), GL_VERTEX_SHADER, "phongModel");

      renderer.glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &subIndex);
      break;
    }
  }
}

bool Model::shouldRotate() const {
  return m_shouldRotate;
}

void Model::setShouldRotate(bool shouldRotate) {
  m_shouldRotate = shouldRotate;
}

#define BUFFER_OFFSET(o) ((const void*) (o))

void Model::initialize(QOpenGLFunctions_4_1_Core* gl) {
  if (m_program != nullptr) delete m_program;
  m_program = new QOpenGLShaderProgram;
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/" + m_shaderName + ".vert");
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/" + m_shaderName + ".frag");

  m_program->link();
  m_program->bind();

  if (!m_vao->isCreated()) m_vao->create();
  m_vao->bind();

  if (!m_vertexVbo->isCreated()) m_vertexVbo->create();
  m_vertexVbo->bind();
  m_vertexVbo->allocate(m_vertices, m_numVertices * 8 * sizeof(float));
  m_program->enableAttributeArray(0);
  m_program->enableAttributeArray(1);
  m_program->enableAttributeArray(2);

  int stride = 8 * sizeof(float);
  gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, stride, BUFFER_OFFSET(0));
  gl->glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, stride, BUFFER_OFFSET(3 * sizeof(float)));
  gl->glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, stride, BUFFER_OFFSET(5 * sizeof(float)));

  if (!m_elementVbo->isCreated()) m_elementVbo->create();
  m_elementVbo->bind();
  m_elementVbo->allocate(m_elements, m_numElements * sizeof(ushort));

  m_vao->release();
  m_vertexVbo->release();
  m_elementVbo->release();
  m_program->release();

  if (m_hasTexture) {
    m_texture = new QOpenGLTexture(QImage(QString(":/assets/textures/mars.jpg")));
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->bind();
  }

  m_isInitialized = true;
}

#undef BUFFER_OFFSET

void Model::destroyResources() {
  if (m_isInitialized) {
    m_vao->destroy();
    m_vertexVbo->destroy();
    m_elementVbo->destroy();
  }
}

void Model::loadFile(const QString& modelFilePath, const QString& shaderName) {
  m_hasTexture = true;
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
    bytes.data(),
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

  if (scene->mNumMeshes == 0) {
    throw std::runtime_error((QString("File contains no meshes: ") + modelFilePath).toStdString());
  }

  aiMesh* mesh = scene->mMeshes[0];
  m_meshData.elementCount = mesh->mNumFaces * 3;

  m_numVertices = mesh->mNumVertices;
  m_vertices = new float[m_numVertices * 8];
  for (uint i = 0; i < mesh->mNumVertices; i++) {
    uint p = i * 8;

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

void Model::translate(float x, float y, float z) {
  m_modelViewMatrix.translate(x, y, z);
}

void Model::rotate(float degree, float x, float y, float z) {
  m_modelViewMatrix.rotate(degree, x, y, z);
}

void Model::setShader(QString shaderName) {
  m_shaderName = shaderName;
  m_isInitialized = false;
}
