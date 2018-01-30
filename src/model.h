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
  Model(bool);
  ~Model();
  virtual void render(QOpenGLFunctions*, const QMatrix4x4&, const long long);
  void loadFile(const QString&, const QString& = "basic");
  void scale(float);
  void translate(float, float, float);
  void rotate(float, float, float, float);

protected:
  QOpenGLShaderProgram* m_program;
  bool m_isInitialized = false;
  QMatrix4x4 m_modelViewMatrix;
  Mesh m_meshData;
  QOpenGLVertexArrayObject* m_vao;
  QOpenGLBuffer* m_vertexVbo = nullptr;
  QOpenGLBuffer* m_elementVbo = nullptr;
  QOpenGLBuffer* m_textureVbo = nullptr;
  QOpenGLTexture* m_texture = nullptr;
  ushort* m_elements = nullptr;
  float* m_textureUVs = nullptr;
  float* m_vertices = nullptr;
  uint m_numElements = 0;
  uint m_numTexels = 0;
  uint m_numVertices = 0;
  float m_rotationSpeed = 90.0f / 1000.0f; // 1/4 rotation per second
  QString m_shaderName = "cube";
  bool m_hasTexture = false;

private:
  void initialize(QOpenGLFunctions* gl);
};

#endif // MODEL_H
