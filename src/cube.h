#ifndef CUBE_H
#define CUBE_H

#include "model.h"

class Cube : public Model {
  Q_OBJECT

public:
  Cube();
  Cube(float, float);
  void render(QOpenGLFunctions*, const QMatrix4x4&, long long);
  void initialize();
  QOpenGLShaderProgram* program();
  void justUpdateUniforms(QOpenGLFunctions*, const QMatrix4x4&, QOpenGLShaderProgram*);
  void release();

protected:
  float m_vertices[12 * 6] = {
    // Front face
    -1.0, -1.0,  1.0,
    1.0, -1.0,  1.0,
    1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,

    // Back face
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
    1.0,  1.0, -1.0,
    1.0, -1.0, -1.0,

    // Top face
    -1.0,  1.0, -1.0,
    -1.0,  1.0,  1.0,
    1.0,  1.0,  1.0,
    1.0,  1.0, -1.0,

    // Bottom face
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,

    // Right face
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    1.0,  1.0,  1.0,
    1.0, -1.0,  1.0,

    // Left face
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0, 1.0, -1.0,
  };

  ushort m_elements[6 * 6] = {
    0,  1,  2,   0,  2,  3,   // front
    4,  5,  6,   4,  6,  7,   // back
    8,  9,  10,  8,  10, 11,  // top
    12, 13, 14,  12, 14, 15,  // bottom
    16, 17, 18,  16, 18, 19,  // right
    20, 21, 22,  20, 22, 23,  // left
  };

};

#endif // CUBE_H
