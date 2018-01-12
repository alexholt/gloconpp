#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <cmath>
#include <exception>
#include <functional>
#include <iomanip>
#include <sstream>

#include <QDomDocument>
#include <QPolygon>
#include <QImage>
#include <QFile>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QPainter>
#include <QRect>
#include <QSvgRenderer>
#include <QVector2D>
#include <QWidget>

#include "model.h"
#include "glocon.h"
#include "territory.h"

#define HALF_HEIGHT 500.0f
#define HALF_WIDTH  1000.0f
#define Z 1.0f

class WorldMap : public Model {
  Q_OBJECT

public:
  WorldMap(const QString&);
  ~WorldMap();
  void render(QOpenGLFunctions*, const QMatrix4x4&, const long long);
  void loadMap();
  QString getShaderName();
  QString getName();
  int getVertexCount();
  QImage* createTexture();

  float m_vertices[6 * 3] = {
    -HALF_WIDTH,  HALF_HEIGHT, Z,
     HALF_WIDTH, -HALF_HEIGHT, Z,
     HALF_WIDTH,  HALF_HEIGHT, Z,
     HALF_WIDTH, -HALF_HEIGHT, Z,
    -HALF_WIDTH,  HALF_HEIGHT, Z,
    -HALF_WIDTH, -HALF_HEIGHT, Z,
  };

  void forEach(std::function<void(QMap<QString, Territory*>::const_iterator)>);

private:
  QVector3D m_position{-1, 0, 1};
  QString m_path;
  QDomDocument m_doc;
  QMap<QString, Territory*> m_territories;
  GLuint m_textureLocation;
  GLuint m_selectedTerritoryIdLocation;
  int m_selectedTerritoryId = -1;
  GLuint m_cameraMatrixLocation;
  GLuint m_positionBuffer;
  GLuint m_texcoordBuffer;
  GLuint m_positionAttributeLocation;
  GLuint m_texcoordAttributeLocation;
  QImage* m_textureData;
  QOpenGLVertexArrayObject* m_vao;
  QOpenGLBuffer* m_vertexVbo;
  QOpenGLBuffer* m_textureVbo;

  GLfloat m_texcoords[12] = {
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
  };

  QOpenGLTexture* m_texture;

public slots:
  void teardownGL();
};

#undef HEIGHT
#undef WIDTH
#undef Z
#endif // WORLDMAP_H
