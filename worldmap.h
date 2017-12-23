#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <cmath>
#include <exception>
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

class WorldMap : public Model {
  Q_OBJECT

public:
  WorldMap(const QString&);
  ~WorldMap();
  void render(QOpenGLFunctions*, const QMatrix4x4&);
  void loadMap();
  QString getShaderName();
  QString getName();
  int getVertexCount();
  QImage* createTexture();

  GLfloat m_vertices[6 * 3] = {
    -1.0f, 0.5f, 0.0f,
    1.0f, 0.5f, 0.0f,
    1.0f, -0.5f, 0.0f,
    1.0f, -0.5f, 0.0f,
    -1.0f, -0.5f, 0.0f,
    -1.0f, 0.5f, 0.0f
  };


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
  QOpenGLBuffer m_vertexVbo;
  QOpenGLBuffer m_textureVbo;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLVertexArrayObject m_texObject;

  GLfloat m_texcoords[12] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
  };

  bool m_isInitialized = false;

  QOpenGLTexture* m_texture;
};

#endif // WORLDMAP_H
