#include "cube.h"

Cube::Cube() :
  m_vao(new QOpenGLVertexArrayObject),
  m_vertexVbo(new QOpenGLBuffer),
  m_elementVbo(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)) {
}

Cube::Cube(float x, float y) :
  m_vao(new QOpenGLVertexArrayObject),
  m_vertexVbo(new QOpenGLBuffer),
  m_elementVbo(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)) {
  m_modelViewMatrix.translate(x, y);
  m_modelViewMatrix.scale(10);
}

void Cube::initialize() {
  m_program = new QOpenGLShaderProgram;
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/cube.vert");
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/cube.frag");
  m_program->link();
  m_program->bind();

  m_vao->create();
  m_vao->bind();

  m_vertexVbo->create();
  m_vertexVbo->bind();
  m_vertexVbo->allocate(m_vertices, sizeof(m_vertices[0]) * 72);
  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

  m_elementVbo->create();
  m_elementVbo->bind();
  m_elementVbo->allocate(m_elements, sizeof(m_elements[0]) * 36);

  m_vao->release();
  m_vertexVbo->release();
  m_elementVbo->release();
  m_program->release();

  m_isInitialized = true;
}

void Cube::render(QOpenGLFunctions* renderer, const QMatrix4x4& cameraMatrix) {
  if (!m_isInitialized) {
    initialize();
  }

  m_program->bind();
  m_vao->bind();

  m_program->setUniformValue("u_camera", cameraMatrix);
  m_program->setUniformValue("u_modelView", m_modelViewMatrix);
  renderer->glDrawElements(GL_TRIANGLES, sizeof(m_elements) / sizeof(m_elements[0]), GL_UNSIGNED_SHORT, 0);

  m_vao->release();
  m_program->release();
}

void Cube::release() {
  m_vao->release();
  m_program->release();
}

void Cube::justUpdateUniforms(QOpenGLFunctions* renderer, const QMatrix4x4& cameraMatrix, QOpenGLShaderProgram* program) {
  program->setUniformValue("u_camera", cameraMatrix);
  program->setUniformValue("u_modelView", m_modelViewMatrix);
  renderer->glDrawElements(GL_TRIANGLES, sizeof(m_elements) / sizeof(m_elements[0]), GL_UNSIGNED_SHORT, 0);
}

void Cube::scale(float factor) {
  m_modelViewMatrix.scale(factor);
}

QOpenGLShaderProgram* Cube::program() {
  return m_program;
}
