#ifndef MODEL_H
#define MODEL_H

#include <exception>
#include <QDebug>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Model : public QObject {
public:
  virtual QString getShaderName() = 0;
  virtual QString getName() = 0;
  virtual int getVertexCount();
  virtual QOpenGLTexture* getTexture();
  GLuint createProgram(QOpenGLFunctions* renderer, QString& vert, QString& frag);
  GLuint createShader(QOpenGLFunctions* renderer, GLenum type, QString& source);

protected:
  QOpenGLShaderProgram* m_program;
  QOpenGLTexture* m_texture;
};

#endif // MODEL_H
