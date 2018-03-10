#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <exception>
#include <QDebug>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QString>

class Model : public QObject {
public:
  Model();
  Model(bool);
  Model(const Model&);
  ~Model();
  virtual void render(QOpenGLFunctions_4_1_Core*, const QMatrix4x4&, const long long);
  void loadFile(const QString&, const QString& = "basic");
  void scale(float);
  void translate(float, float, float);
  void rotate(float, float, float, float);
  void setShader(QString);

  bool shouldRotate() const;
  void setShouldRotate(bool shouldRotate);
  void destroyResources();

protected:
  void setUniforms(const QMatrix4x4&, QOpenGLFunctions_4_1_Core&);

  QOpenGLShaderProgram* m_program = nullptr;
  bool m_isInitialized = false;
  QMatrix4x4 m_modelViewMatrix;
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
  float m_rotationSpeed = 45.0f / 1000.0f;
  QString m_shaderName = "cube";
  bool m_hasTexture = false;
  bool m_shouldRotate = false;
  bool m_canRender = true;
  QMutex m_canRenderMutex;

private:
  void initialize(QOpenGLFunctions_4_1_Core* gl);

  QList<QString> m_shaderNames = {"basic"};
  QMap<QString, QOpenGLShaderProgram*> m_shaders;
};

#endif // MODEL_H
