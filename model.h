#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <exception>
#include <QDebug>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QString>

struct Mesh {
  QOpenGLVertexArrayObject* vao;
  QOpenGLBuffer* vbo;
  QOpenGLBuffer* indices;
  int elementCount;
};

class Model : public QObject {
public:
  Model();
  ~Model();
  virtual void render(QOpenGLFunctions*, const QMatrix4x4&, const long long);
  GLuint createProgram(QOpenGLFunctions* renderer, QString& vert, QString& frag);
  GLuint createShader(QOpenGLFunctions* renderer, GLenum type, QString& source);
  void translate(float, float, float);
  void loadFile(const QString&, const QString& = "basic");
  void scale(float);

protected:
  QOpenGLShaderProgram* m_program;
  QOpenGLTexture* m_texture;
  bool m_isInitialized = false;
  QMatrix4x4 m_modelViewMatrix;
  Mesh m_meshData;
  QOpenGLVertexArrayObject* m_vao;
  QOpenGLBuffer* m_vertexVbo;
  QOpenGLBuffer* m_elementVbo;
  ushort* m_elements = nullptr;
  float* m_vertices = nullptr;
  uint m_numElements = 0;
  uint m_numVertices = 0;
  float m_rotationSpeed = 90.0f / 1000.0f; // 1/4 rotation per second
  QString m_shaderName = "cube";

private:
  void initialize();
};

#endif // MODEL_H
