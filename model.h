#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <exception>
#include <QDebug>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

class Model : public QObject {
public:
  virtual void render(QOpenGLFunctions*, const QMatrix4x4&) = 0;
  GLuint createProgram(QOpenGLFunctions* renderer, QString& vert, QString& frag);
  GLuint createShader(QOpenGLFunctions* renderer, GLenum type, QString& source);
  void translate(float, float, float);

protected:
  QOpenGLShaderProgram* m_program;
  QOpenGLTexture* m_texture;
  bool m_isInitialized = false;
  QMatrix4x4 m_modelViewMatrix;
};

#endif // MODEL_H
